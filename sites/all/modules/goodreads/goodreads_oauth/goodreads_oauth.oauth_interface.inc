<?php

/**
 * @file
 * Implement Goodreads OAuth authorization interface.
 *
 * This is using the OAuth module's "API," such as it is.  Once I had done a
 * bunch of it using code from the LinkedIn and Twitter modules as an example.
 * I then recognized the code in the OAuth module's includes directory.
 * From that I have been able to refactor this code to better utilize it.
 * I still think I have a long way to go to be using it "right."
 *
 * Basically there are two functions called by any other code in the Goodreads
 * module, they are
 *   function goodreads_oauth_request_access_token()
 *     used to get step through the process
 *   function goodreads_oauth_authorize_callback()
 *     set up in goodreads_oauth.module as the
 *     callback function called when Goodreads returns to goodreads/token/%user
 *
 * goodreads_oauth_request_access_token() walks through the three step process
 * to get an OAuth access token for the current user:
 * 1.  Get a request token using the Goodreads API keys stored as
 *     part of the base Goodreads settings
 * 2.  Redirect the user to the Goodreads site to authorize access,
 *     which returns viThe goodreads_oauth_authorize_callback function
 * 3.  Exchange the request token that has been authorized for an access token.
 *
 * Assuming all goes well, if the user is logged into the Drupal site the
 * access token is saved in a table {goodreads_oauth_token}, otherwise it
 * is saved in the user's session cookie. It is possible that
 * {goodreads_oauth_token} duplicates information stored in another table
 * by the OAuth module, but I don't see an obvious way to get to it by UID,
 * although I saw a patch in an issue that appears to add that capability.
 */


/**
 * Get a request token from Goodreads.
 *
 * @return array
 *   FALSE if no access token is saved, otherwise the process will return to
 */
function goodreads_oauth_request_access_token($return_url = NULL) {
  global $user;
  // Set up for our function.
  // If the Goodreads API keys are not set, that's a deal breaker.
  if (!_goodreads_oauth_ready()) {
    return FALSE;
  }
  // Save return url in session cookie,
  // default to current page if not specified.
  if ($return_url != NULL) {
    $_SESSION['goodreads_oauth']['return_url'] = $return_url;
  }
  else {
    $_SESSION['goodreads_oauth']['return_url'] = request_uri();
  }
  $anon = ($user->uid < 1) ? TRUE : FALSE;
  // Set up for OAuth module API calls.
  $consumer = DrupalOAuthConsumer::load(variable_get('goodreads_api_key', ''), FALSE);
  if (!$consumer) {
    $consumer = new DrupalOAuthConsumer(variable_get('goodreads_api_key', ''), variable_get('goodreads_api_secret', ''));
    $consumer->write();
  }
  // **** This is step 1 of the 3 step OAuth process.
  $sig_method = DrupalOAuthClient::signatureMethod();
  $client = new DrupalOAuthClient($consumer, NULL, $sig_method, OAUTH_COMMON_VERSION_1);
  // Get the reqest token.
  $request_token_url = GOODREADS_ENDPOINT . 'oauth/request_token';
  $request_token = $client->getRequestToken($request_token_url);
  // Save the request token key for OAuth module.
  $request_token->write();
  // Save the request token key for our module.
  // I think this is totally redundant to the OAuth module and
  // need to look at removing it.
  if ($request_token->key) {
    if (!$anon) {
      // Logged in user. Store request token for next step in
      // db instead of $_SESSION for security.
      // db_query("DELETE FROM {goodreads_oauth_token}
      // WHERE uid = %d", $user->uid);
      db_delete('goodreads_oauth_token')
        ->condition('uid', $user->uid)
        ->execute();
      $sql = array(
        'uid' => $user->uid,
        'gid' => '',
        'token_key' => $request_token->key,
        'token_secret' => $request_token->secret,
        'type' => 'request',
      );
      drupal_write_record('goodreads_oauth_token', $sql);
    }
    else {
      // Anonymous user. We can't use db storage,
      // but $_SESSION can not lead to identity forgery for anonymous.
      $_SESSION['goodreads_oauth']['oauth_token'] = $request_token->key;
      $_SESSION['goodreads_oauth']['oauth_token_secret'] = $request_token->secret;
    }
  }
  else {
    // If ($request_token->key)
    drupal_set_message(t('There was a problem with the configuration of Goodreads on this website.'), 'error');
    watchdog('Goodreads OAuth', 'The website could not communicate with Goodreads. It is likely your API credentials are misconfigured.', array(), WATCHDOG_ERROR);
    return FALSE;
  }
  // **** This is step 2 of the 3 step OAuth process.
  // Now we have the request token, have the user authorize us.
  $goodreads_authorization_url = GOODREADS_ENDPOINT . 'oauth/authorize';
  $drupal_callback_url = url('goodreads/token/' . $user->uid, array('absolute' => TRUE));
  $auth_url = $client->getAuthorizationUrl($goodreads_authorization_url, array('callback' => $drupal_callback_url));
  // Go to the Goodreads authorization page,
  // when the user accepts or rejects the authorization, return to
  // goodreads_oauth_authorize_callback().
  drupal_goto($auth_url);
}


/**
 * Callback for Goodreads OAuth authorize to return to.
 *
 * @return array
 *   For lack of a better idea, we'll return to the user page.
 */
function goodreads_oauth_authorize_callback() {
  global $user;
  $anon = ($user->uid < 1) ? TRUE : FALSE;
  // Reload the OAuth module objects.
  $consumer = DrupalOAuthConsumer::load(variable_get('goodreads_api_key', ''), FALSE);
  $db_result = db_select('goodreads_oauth_token', 'oauth')
    ->condition('uid', $user->uid, '=')
    ->fields('oauth', array('uid', 'gid', 'token_key', 'token_secret'))
    ->execute()
    ->fetchAll();
  $request_token = DrupalOAuthToken::loadByKey($db_result[0]->token_key, $consumer, OAUTH_COMMON_TOKEN_TYPE_REQUEST);
  // Did we get authorized?
  if ($_GET['authorize'] == 1) {
    // Goodreads user authorized access, get access token,
    // tell user and GO WHERE???
    // **** This is step 3 of the 3 step OAuth process.
    $access_token_url = GOODREADS_ENDPOINT . 'oauth/access_token';
    $client = new DrupalOAuthClient($consumer, $request_token);
    $verifier = isset($_GET['oauth_verifier']) ? $_GET['oauth_verifier'] : NULL;
    $access_token = $client->getAccessToken($access_token_url, array('verifier' => $verifier));
    if (!is_null($request_token)) {
      $request_token->delete();
    }
    $access_token->write();
    if (!$anon) {
      // Logged in user.  Store access token for next step in
      // db instead of $_SESSION for security.
      // db_query("DELETE FROM {goodreads_oauth_token}
      // WHERE uid = %d", $user->uid);
      db_delete('goodreads_oauth_token')
        ->condition('uid', $user->uid)
        ->execute();
      $sql = array(
        'uid' => $user->uid,
        'gid' => '',
        'token_key' => $access_token->key,
        'token_secret' => $access_token->secret,
        'type' => 'access',
      );
      drupal_write_record('goodreads_oauth_token', $sql);
      // Now get the Goodreads user ID from Goodreads and
      // save it in the database too.
      $gid = goodreads_oauth_get_user_id();
      // db_query("UPDATE {goodreads_oauth_token} SET gid = '%s'
      // WHERE uid = %d", $gid['id'], $user->uid);
      db_update('goodreads_oauth_token')
        ->fields(array('gid' => $gid['id']))
        ->condition('uid', $user->uid)
        ->execute();
    }
    else {
      // Anonymous user. We can't use db storage,
      // but $_SESSION can not lead to identity forgery for anonymous.
      $_SESSION['goodreads_oauth']['oauth_token'] = $oauth['oauth_token'];
      $_SESSION['goodreads_oauth']['oauth_token_secret'] = $oauth['oauth_token_secret'];
    }
    drupal_set_message(t('Your Goodreads authorization has been saved.'), 'status');
    drupal_goto('/user/' . $user->uid . '/edit/goodreads');
  }
  else {
    // Goodreads user didn't authorize access, delete request token,
    // tell user and GO WHERE???
    // Whether logged in or not, tell OAuth module to
    // delete useless request key.
    if (!is_null($request_token)) {
      $request_token->delete();
    }
    if (!$anon) {
      // Logged in user : delete request token from db.
      // db_query("DELETE FROM {goodreads_oauth_token}
      // WHERE uid = %d", $user->uid);
      db_delete('goodreads_oauth_token')
        ->condition('uid', $user->uid)
        ->execute();
    }
    else {
      // Anonymous user. Delete request token from $_SESSION can not
      // lead to identity forgery for anonymous.
      $_SESSION['goodreads_oauth']['oauth_token'] = '';
      $_SESSION['goodreads_oauth']['oauth_token_secret'] = '';
    }
    drupal_set_message(t('The Goodreads user did not authorize use of their account.'), 'error');
    drupal_goto('/user/' . $user->uid . '/edit/goodreads');
  }
}
