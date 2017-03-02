<?php

/**
 * @file
 * Goodreads OAuth support functions other than the OAuth authorization
 *   sequence which is in the goodreads_oath.oauth_interface.inc file.
 *
 * @todo use _goodreads_oauth_api instead of _goodreads_oauth_get_api,
 *   _goodreads_oauth_post_api, _goodreads_oauth_put_api, and
 *   _goodreads_oauth_delete_api
 */


/**
 * API functions in this file:
 * goodreads_oauth_get_user_id()
 * goodreads_oauth_get_user_notifications() **
 * goodreads_oauth_get_user_friends($goodreads_id = '', $page = '', $sort = '',
 *   $max_pages = GOODREADS_MAX_PAGES)
 * goodreads_oauth_get_user_friends_updates($update_filter = '',
 *   $max_updates = '', $update_type = '')
 * goodreads_oauth_get_users_owned_books($user_id, $page = '',
 *   $max_pages = GOODREADS_MAX_PAGES)
 * goodreads_oauth_get_user_compare_books($other_user_id = '1')
 * goodreads_oauth_get_show_owned_book($book_id = '1')
 * goodreads_oauth_get_topics_with_unread_comments($group_id,
 *   $viewed = '', $sort = '', $order = '', $page = '',
 *   $max_pages = GOODREADS_MAX_PAGES)
 * goodreads_oauth_get_user_follower_list($user_id, $page = '',
 *   $max_pages = GOODREADS_MAX_PAGES) **
 * goodreads_oauth_get_user_following_list($user_id, $page = '',
 *   $max_pages = GOODREADS_MAX_PAGES) **
 * goodreads_oauth_add_bookshelf($name, $featured = '',
 *   $exclusive = '', $sortable = '') **
 * goodreads_oauth_edit_bookshelf($shelf_id, $new_name = '',
 *   $featured = '', $exclusive = '', $sortable = '')
 * goodreads_oauth_delete_bookshelf($shelf_id)
 * goodreads_oauth_follow_user($user_id, $my_user_id = '') **
 * goodreads_oauth_add_friend($user_id)
 * goodreads_oauth_delete_status($status_id = '')
 * goodreads_oauth_add_book_to_shelf($book_id,
 *   $shelf_name = '', $delete = '')
 * goodreads_oauth_update_user_status($book_id = '', $percentage = '',
 *   $page = '', $message = '')
 * goodreads_oauth_rate_review($resource_id = '', $like = '')
 * goodreads_oauth_create_comment($resource_id =  NULL,
 *   $type = '', $comment = '')
 * goodreads_oauth_add_owned_book($book_id = '', $condition = '',
 *   $condition_description = '', $bcid = '', $purchase_location = '',
 *   $purchase_date = '', $available_for_swap = '')
 * goodreads_oauth_update_owned_book($owned_book_id = '', $condition = '',
 *   $condition_description = '', $bcid = '', $purchase_location = '',
 *   $purchase_date = '', $available_for_swap = '')
 * goodreads_oauth_add_review($book_id = '', $rating = '', $review_text = '',
 *   $shelf_name = '', $date_read = '')
 * goodreads_oauth_edit_review($book_id = '', $rating = '', $review_text = '',
 *   $shelf_name = '', $date_read = '') **
 * goodreads_oauth_destroy_review($book_id = '')
 * goodreads_oauth_add_quote($quote = '', $author_name = '', $book_id = '',
 *   $isbn = '', $author_id = '', $tags = '')
 * goodreads_oauth_create_topic($title = '', $subject_type = '',
 *   $subject_id = '', $comment = '', $update_feed = '', $digest = '',
 *   $folder_id = '', $question_flag = '')
 * goodreads_oauth_get_user_books($user_id = '', $sort = '', $order = '',
 *   $shelf_name = '', $query = '', $per_page = '', $page = '',
 *   $max_pages = GOODREADS_MAX_PAGES)
 * goodreads_oauth_get_user_books_all_shelves($user_id,
 *   $maximum_books_per_shelf = 100)
 * goodreads_oauth_use_another_user($uid)
 * goodreads_oauth_user_current_user()
 *
 * ** indicates there is some serious issue with the API call,
 * see function definition for details.
 */


/**
 * Get the current user's Goodreads ID using OAuth.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or an array with the user's Goodreads information.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_get_user_id() {
  // Make the API call.
  $xml = _goodreads_oauth_get_api('api/auth_user');
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($xml['api_error'] === TRUE) {
    return $xml;
  }
  else {
    $xml = $xml['data'];
  }
  $api_result_array = _goodreads_xml2array($xml);
  return array(
    'api_error' => FALSE,
    'id' => $api_result_array['GoodreadsResponse']['user']['user_attr']['id'],
    'name' => $api_result_array['GoodreadsResponse']['user']['name'],
    'link' => $api_result_array['GoodreadsResponse']['user']['link'],
    'original_xml' => $xml,
  );
}


// ********************* CURRENTLY BROKEN, gets a 401 unauthorized error.
/**
 * Get the current user's Goodreads notifications using OAuth.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or an array with the user's Goodreads information.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_get_user_notifications() {
  // Make the API call.
  $xml = _goodreads_oauth_get_api('notifications.xml');
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($xml['api_error'] === TRUE) {
    return $xml;
  }
  else {
    $xml = $xml['data'];
  }
  $api_result_array = _goodreads_xml2array($xml);
  $function_result['original_xml'][] = $xml;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * Get a list of the current user's Goodreads FRIENDS using OAuth.
 *
 * @param string $goodreads_id
 *   Goodreads user_id. (required by Goodreads)
 *   If missing, current Drupal user's Goodreads ID will be used
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   Unlike other API's this one seems to return 10 friends per page
 *   If missing, ALL of the user's friends will be downloaded,
 *   very likely that $max_pages default will cut off list at 50.
 * @param string $sort
 *   How to sort.  Options are: last_online|date_added|first_name (optional)
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or an array with the user's Goodreads information.
 *   If the user does not have any friends, then ['api_error']=FALSE but no
 *   data is returned.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_get_user_friends($goodreads_id = '', $page = '', $sort = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up parameter array for API call.
  $parameters = array();
  if ($goodreads_id === '') {
    goodreads_oauth_get_user_id();
    $current_user_id = $current_user_id['id'];
    $parameters['id'] = $current_user_id['id'];
    if ($parameters['id'] === FALSE) {
      // If there is a problem getting the current Drupal user's
      // Goodreads ID, return with failure.
      return FALSE;
    }
  }
  else {
    $parameters['id'] = $goodreads_id;
  }
  if ($page === '') {
    $parameters['page'] = 1;
    $get_all = TRUE;
  }
  else {
    $parameters['page'] = $page;
    $get_all = FALSE;
  }
  if ($sort != '') {
    $parameters['sort'] = $sort;
  }
  $api_xml_result_all = array();
  // Make the API call.
  $xml = _goodreads_oauth_get_api('friend/user.xml', $parameters);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($xml['api_error'] === TRUE) {
    return $xml;
  }
  else {
    $xml = $xml['data'];
  }
  $api_xml_result_all[] = $xml;
  $api_result_array = _goodreads_xml2array($xml);
  $function_result = $api_result_array['GoodreadsResponse'];
  $number_pages = isset($api_result_array['GoodreadsResponse']['friends']['friends_attr']['total']) ? floor($api_result_array['GoodreadsResponse']['friends']['friends_attr']['total'] / 50) : 0;
  if ($number_pages > $max_pages) {
    // Limit results to $max_pages pages.
    $number_pages = $max_pages;
  }
  if ($get_all && ($number_pages > 0)) {
    // Get the other pages.
    for ($i = 1; $i <= $number_pages; $i++) {
      $parameters['page']++;
      // Make the API call.
      $xml = _goodreads_oauth_get_api('friend/user.xml', $parameters);
      // If something went wrong in the call, return ['api_error'] = TRUE.
      if ($xml['api_error'] === TRUE) {
        return $xml;
      }
      else {
        $xml = $xml['data'];
      }
      $api_xml_result_all[] = $xml;
      $api_result_array = _goodreads_xml2array($xml);
      // Cycle through each additional friend and append since returned
      // values have same index as last API call.
      foreach ($api_result_array['GoodreadsResponse']['friends']['user'] as $another_friend) {
        $function_result['friends']['user'][] = $another_friend;
      }
    }
  }
  $function_result['original_xml'] = $api_xml_result_all;
  $function_result['api_error'] = FALSE;
  return $function_result;
}


/**
 * Get the current user's Goodreads friends' UPDATES using OAuth.
 *
 * @param string $update_filter
 *   Which updates to show. Options are:
 *   friends (default, includes followers),
 *   following, top_friends. (optional)
 * @param string $max_updates
 *   The max limit of updates. (optional)
 * @param string $update_type
 *   Type of update. Options are: books, reviews, statuses. (optional)
 *   default is all
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   an array with the user's Goodreads information.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_get_user_friends_updates($update_filter = '', $max_updates = '', $update_type = '') {
  // Set up parameter array for API call.
  $parameters = array();
  if ($update_filter != '') {
    $parameters['update_filter'] = $update_filter;
  }
  if ($max_updates != '') {
    $parameters['max_updates'] = $max_updates;
  }
  if ($update_type != '') {
    $parameters['update'] = $update_type;
  }
  // Make the API call.
  $xml = _goodreads_oauth_get_api('updates/friends.xml', $parameters);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($xml['api_error'] === TRUE) {
    return $xml;
  }
  else {
    $xml = $xml['data'];
  }
  $api_result_array = _goodreads_xml2array($xml);
  $api_result_array = $api_result_array['GoodreadsResponse']['updates'];
  $api_result_array['api_error'] = FALSE;
  $api_result_array['original_xml'] = $xml;
  return $api_result_array;
}


/**
 * List books owned by a user.
 *
 * @param string $user_id
 *   The Goodreads user ID.
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the user's books will be downloaded.
 * @param string $max_pages
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or multi-dimensional array with result of request.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_get_users_owned_books($user_id, $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up parameter array for API call.
  $params = array(
    'id' => $user_id,
    'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($page === '') {
    $params['page'] = 1;
    $get_all = TRUE;
  }
  else {
    $params['page'] = $page;
    $get_all = FALSE;
  }
  $api_result_array = array();
  $api_xml_result_all = array();
  // Loop through the results.
  do {
    // Make the API call.
    $api_xml_result = _goodreads_oauth_get_api('owned_books/user', $params);
    // If something went wrong in the call, return ['api_error'] = TRUE.
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    $api_result = $api_result['GoodreadsResponse']['owned_books'];
    $result_count = isset($api_result['user_shelf']) ? count($api_result['user_shelf']) : 0;
    if (($params['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result.
      $api_result_array = $api_result;
    }
    if ($result_count != 0) {
      foreach ($api_result['owned_book'] as $book) {
        $api_result_array['owned_book'][] = $book;
      }
      $params['page']++;
      if ($get_all && $params['page'] > $max_pages) {
        // After getting $max_pages pages, force the loop to end by
        // setting $result_count to 0.
        $result_count = 0;
      }
    }
  } while ($get_all && ($result_count != 0));

  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * OAuth compare books.
 *
 * Compare books between the current user's and any other Goodreads user
 * using OAuth.
 *
 * @param string $other_user_id
 *   The other Goodreads user ID
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or multi-dimensional array with result of request.
 *   Trying to compare with yourself returns an error
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_get_user_compare_books($other_user_id = '1') {
  // Make the API call.
  $xml = _goodreads_oauth_get_api('user/compare/' . $other_user_id . '.xml');
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($xml['api_error'] === TRUE) {
    return $xml;
  }
  else {
    $xml = $xml['data'];
  }
  $api_result_array = _goodreads_xml2array($xml);
  $api_result_array = $api_result_array['GoodreadsResponse']['compare'];
  $api_result_array['original_xml'] = $xml;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * Show an owned book.
 *
 * @param string $book_id
 *   The Goodreads book ID.  If the user doesn't own this book, returns
 *   ['api_error']=TRUE as a result of a 404 error.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or multi-dimensional array with result of request.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_get_show_owned_book($book_id = '1') {
  // Make the API call.
  $xml = _goodreads_oauth_get_api('owned_books/' . $book_id . '.xml');
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($xml['api_error'] === TRUE) {
    return $xml;
  }
  else {
    $xml = $xml['data'];
  }
  $api_result_array = _goodreads_xml2array($xml);
  $api_result_array = $api_result_array['GoodreadsResponse']['owned_book']['owned_book'];
  $api_result_array['original_xml'] = $xml;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * Get a list of topics with unread comments for the current OAuth user.
 *
 * @param string $group_id
 *   The Goodreads group ID.
 * @param string $viewed
 *   Indicates whether to show topics user has viewed before or not.
 *   'true' or 'false', default is 'false' (include all topics).
 *   Set this param to 'true' or '1' to restrict to only topics the user
 *     has already viewed.
 * @param string $sort
 *   Field to sort topics by.
 *   One of 'title', 'updated_at', 'views', 'comments_count'
 * @param string $order
 *   'a' for ascending, 'd' for descending
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the user's books will be downloaded.
 * @param string $max_pages
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or multi-dimensional array with result of request.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_get_topics_with_unread_comments($group_id, $viewed = '', $sort = '', $order = '', $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up parameter array for API call.
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($viewed != '') {
    $params['viewed'] = $viewed;
  }
  if ($sort != '') {
    $params['sort'] = $sort;
  }
  if ($order != '') {
    $params['order'] = $order;
  }
  if ($page === '') {
    $params['page'] = 1;
    $get_all = TRUE;
  }
  else {
    $params['page'] = $page;
    $get_all = FALSE;
  }
  $api_result_array = array();
  $api_xml_result_all = array();
  // Loop through the results.
  do {
    // Make the API call.
    $api_xml_result = _goodreads_oauth_get_api('topic/unread_group/' . $group_id . '.xml', $params);
    // If something went wrong in the call, return ['api_error'] = TRUE.
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    $api_result = $api_result['GoodreadsResponse']['group_folder'];
    $result_count = isset($api_result['topics']['topic']) ? count($api_result['topics']['topic']) : 0;
    if (($params['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result.
      $api_result_array = $api_result;
    }
    else {
      // Otherwise getting additional pages of books,
      // append them to the books array.
      if ($result_count != 0) {
        foreach ($api_result['topics']['topic'] as $topic) {
          $api_result_array['topics']['topic'][] = $topic;
        }
      }
    }
    $params['page']++;
    if ($get_all && $params['page'] > $max_pages) {
      // After getting $max_pages pages, force the loop to end by
      // setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));
  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


// ********************* CURRENTLY BROKEN, gets a 401 unauthorized error
// ********* There is a non-OAuth version of the function in goodreads.inc.
/**
 * Get people who are following a user.
 *
 * @param string $user_id
 *   Goodreads user ID.
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the user's books will be downloaded.
 * @param string $max_pages
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or multi-dimensional array with result of request.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_get_user_follower_list($user_id, $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up parameter array for API call.
  $params = array(
  // 'format' => 'xml',
  // 'key' => variable_get('goodreads_api_key', '')
  );
  if ($page === '') {
    $params['page'] = 1;
    $get_all = TRUE;
  }
  else {
    $params['page'] = $page;
    $get_all = FALSE;
  }
  $api_result_array = array();
  $api_xml_result_all = array();
  // Loop through the results.
  do {
    // Make the API call.
    $api_xml_result = _goodreads_oauth_get_api('user/' . $user_id . '/followers.xml', $params);
    // If something went wrong in the call, return ['api_error'] = TRUE.
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    $api_result = $api_result['GoodreadsResponse'];
    $result_count = isset($api_result['books']['book']) ? count($api_result['books']['book']) : 0;
    if (($params['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result.
      $api_result_array = $api_result;
    }
    else {
      // Otherwise getting additional pages of books,
      // append them to the books array.
      if ($result_count != 0) {
        foreach ($api_result['books']['book'] as $book) {
          $api_result_array['books']['book'][] = $book;
        }
      }
    }
    $params['page']++;
    if ($get_all && $params['page'] > $max_pages) {
      // After getting $max_pages pages, force the loop to end by
      // setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));

  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


// ***** CURRENTLY BROKEN, gets a 401 unauthorized error
// ***** There is a non-OAuth version of the function in goodreads.inc
/**
 * Get people a user is following.
 *
 * @param string $user_id
 *   The Goodreads user ID.
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the user's books will be downloaded.
 * @param string $max_pages
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   otherwise return['original_xml'] contains an array of original XML
 *      responses, just in case.
 */
function goodreads_oauth_get_user_following_list($user_id, $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up parameter array for API call.
  $params = array(
    'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($page === '') {
    $params['page'] = 1;
    $get_all = TRUE;
  }
  else {
    $params['page'] = $page;
    $get_all = FALSE;
  }
  $api_result_array = array();
  $api_xml_result_all = array();
  // Loop through the results.
  do {
    // Make the API call.
    $api_xml_result = _goodreads_oauth_get_api('user/' . $user_id . '/following', $params);
    // If something went wrong in the call, return ['api_error'] = TRUE.
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    $api_result = $api_result['GoodreadsResponse'];
    $result_count = isset($api_result['books']['book']) ? count($api_result['books']['book']) : 0;
    if (($params['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result.
      $api_result_array = $api_result;
    }
    else {
      // Otherwise getting additional pages of books,
      // append them to the books array.
      if ($result_count != 0) {
        foreach ($api_result['books']['book'] as $book) {
          $api_result_array['books']['book'][] = $book;
        }
      }
    }
    $params['page']++;
    if ($get_all && $params['page'] > $max_pages) {
      // After getting $max_pages pages, force the loop to end by
      // setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));

  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


// *********** Bookshelf flags: featured, exclusive, sortable,
// appear to not set when the API is called.
/**
 * Add a book shelf using OAuth.
 *
 * From the Goodreads Website (http://www.goodreads.com/shelf/edit):
 *
 * Featured:  You may choose one shelf to feature on the top of your profile.
 *   If the shelf is sortable, then the top books are shown on the profile.
 *   If it's not sortable then we choose random books from the list. If you
 *   choose the read shelf (default setting), we randomly display 5 star
 *   rated books. You may also choose to have no shelf displayed, by
 *   unchecking the currently featured shelf.
 *
 * Exclusive
 *   All members have three default shelves (read, currently-reading, and
 *   to-read), which are mutually exclusive, meaning a book can only be on
 *   one of them. Members can create as many additional shelves as they like,
 *   and books can be on as many non-exclusive shelves as you desire.
 *   This may be useful for shelves such as "reference" or "gave-up-on."
 *
 * Sortable
 *   You can enable shelf sorting to customize the order of your shelves.
 *   This is extremely useful for the "to-read" shelf;
 *   You can keep track of what you'd like to read next!
 *
 * @param string $name
 *   Name of the bookshelf.
 * @param string $featured
 *   Whether the bookshelf is featured. (optional)
 *   'true' or 'false', default is false.
 * @param string $exclusive
 *   Whether the bookshelf is exclusive. (optional)
 *   'true' or 'false', default is false.
 * @param string $sortable
 *   Whether the bookshelf is sortable. (optional)
 *   'true' or 'false', default is false.
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_add_bookshelf($name, $featured = '', $exclusive = '', $sortable = '') {
  // Set up parameter array for API call.
  $params = array(
    'user_shelf[name]' => $name,
  );
  if ($featured != '') {
    $params['user_shelf[featured]'] = $featured;
  }
  if ($exclusive != '') {
    $params['user_shelf[exclusive_flag]'] = $exclusive;
  }
  if ($sortable != '') {
    $params['user_shelf[sortable_flag]'] = $sortable;
  }
  // Make the API call.  drupal_query_string_encode($parameters)
  $result = _goodreads_oauth_post_api('user_shelves.xml', $params);
  return $result;
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($result['api_error'] === TRUE) {
    return $result;
  }
  else {
    return $result;
  }
}


/**
 * Edit a book shelf using OAuth.
 *
 * From the Goodreads Website (http://www.goodreads.com/shelf/edit):
 *
 * Featured:  You may choose one shelf to feature on the top of your profile.
 *   If the shelf is sortable, then the top books are shown on the profile.
 *   If it's not sortable then we choose random books from the list. If you
 *   choose the read shelf (default setting), we randomly display 5 star
 *   rated books. You may also choose to have no shelf displayed, by
 *   unchecking the currently featured shelf.
 *
 * Exclusive
 *   All members have three default shelves (read, currently-reading, and
 *   to-read), which are mutually exclusive, meaning a book can only be on
 *   one of them. Members can create as many additional shelves as they like,
 *   and books can be on as many non-exclusive shelves as you desire.
 *   This may be useful for shelves such as "reference" or "gave-up-on."
 *
 * Sortable
 *   You can enable shelf sorting to customize the order of your shelves.
 *   This is extremely useful for the "to-read" shelf;
 *   You can keep track of what you'd like to read next!
 *
 * @param string $shelf_id
 *   Current name of the bookshelf.
 * @param string $new_name
 *   New name of the bookshelf.
 *   if you try to rename a shelf to a name that you already have you will
 *   get an 'api_error' = true and a 'template not found' data or
 *   'not found' response message
 * @param string $featured
 *   Whether the bookshelf is featured. (optional)
 *   'true' or 'false', default is false.
 * @param string $exclusive
 *   Whether the bookshelf is exclusive. (optional)
 *   'true' or 'false', default is false.
 * @param string $sortable
 *   Whether the bookshelf is sortable. (optional)
 *   'true' or 'false', default is false.
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_edit_bookshelf($shelf_id, $new_name = '', $featured = '', $exclusive = '', $sortable = '') {
  // Set up parameter array for API call.
  $params = array(
    'user_shelf[name]' => $new_name,
  );
  if ($featured != '') {
    $params['user_shelf[featured]'] = $featured;
  }
  if ($exclusive != '') {
    $params['user_shelf[exclusive_flag]'] = $exclusive;
  }
  if ($sortable != '') {
    $params['user_shelf[sortable_flag]'] = $sortable;
  }
  // This would have been the API if the documentation were right and
  // we'd be using the shelf name.
  // $result = _goodreads_oauth_put_api('user_shelves/%5B'. $old_name .
  // '%5D.xml', $params);
  // Make the API call.
  $result = _goodreads_oauth_put_api('user_shelves/' . $shelf_id . '.xml', $params);
  return $result;
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($result['api_error'] === TRUE) {
    return $result;
  }
  else {
    return $result;
  }
}


/**
 * Delete a book shelf using OAuth.
 *
 * @param string $shelf_id
 *   The Goodreads shelf ID of the bookshelf to delete.
 *   to find the shelf ID call
 *     goodreads_get_user_info($user_id = '', $username = '')
 *     and look in the [user_shelves][user_shelf] arrays (0, 1, 2...) for the
 *       name in [user_shelves][user_shelf][n][name] then get the shelf id from
 *        [user_shelves][user_shelf][n]['id']
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_delete_bookshelf($shelf_id) {
  // Set up parameter array for API call.
  $params = array();
  // This would have been the API if the documentation were right and
  // we'd be using the shelf name.
  // $result = _goodreads_oauth_delete_api('user_shelves/%5B' .
  // $name . '%5D.xml', $params);
  // Make the API call.
  $result = _goodreads_oauth_delete_api('user_shelves/' . $shelf_id . '.xml', $params);
  return $result;
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($result['api_error'] === TRUE) {
    return $result;
  }
  else {
    return $result;
  }
}


// *************** something wrong with API documentation,
// always returns OK response with data =
// '$("follow_container_user_7535600").
// update("Something went wrong! Follower can't be blank");'
/**
 * Follow a user using OAuth.
 *
 * @param string $user_id
 *   The Goodreads user ID to follow.
 *   (the person you want to follow) (required)
 * @param string $my_user_id
 *   The Goodreads user ID of the follower.
 *   (this could get confusing, the user who will be following
 *     the user specified by $user_id)
 *   if NULL, will use the current OAuth user's Goodreads user ID.
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_follow_user($user_id, $my_user_id = '') {
  // Set up parameter array for API call.
  // FWIW when I had commented out the $user_id parameter, I got an OK
  // response with data = '$("follow_container_user_7535600").
  // update("Something went wrong! Follower can't be blank");'
  $params = array(
    // 'id' => $user_id,
  );
  // If ($my_user_id === '') {
  // $my_user_id = goodreads_oauth_get_user_id();
  // $my_user_id = $my_user_id['id'];
  // }
  // Make the API call.
  $result = _goodreads_oauth_post_api('user/' . $user_id . '/followers', $params);
  return $result;
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($result['api_error'] === TRUE) {
    return $result;
  }
  else {
    return $result;
  }
}


/**
 * Add a friend using OAuth.
 *
 * @param string $user_id
 *   The Goodreads user ID.
 *
 * @return array
 *   array with the status.  Call doesn't appear to return any status message.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_add_friend($user_id) {
  // Set up parameter array for API call.
  $params = array(
    'id' => $user_id,
  );
  // Make the API call.
  $result = _goodreads_oauth_post_api('friend/add_as_friend.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($result['api_error'] === TRUE) {
    return $result;
  }
  else {
    return $result;
  }
}


/**
 * Delete a status update using OAuth.
 *
 * @param string $status_id
 *   The Goodreads status ID.
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 *   It would appear that if you try to delete a status update of another
 *     Goodreads user, the API will report 'OK', e.g. no error, but ignore
 *     the request.
 *   If you try to delete a status update you have already deleted, or that
 *     doesn't exist, you'll get an 'api_error'=TRUE and a 404 error code.
 */
function goodreads_oauth_delete_status($status_id = '') {
  // Set up parameter array for API call.
  $params = array(
  // 'id' => $status_id,
  );
  // Make the API call.
  $result = _goodreads_oauth_post_api('user_status/destroy/' . $status_id . '.xml', $params);
  return $result;
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($result['api_error'] === TRUE) {
    return $result;
  }
  else {
    return $result;
  }
}


/**
 * Add a book to a shelf using OAuth.
 *
 * @param string $book_id
 *   The Goodreads book ID.
 * @param string $shelf_name
 *   The name of the shelf to add the book to.
 *   to get the shelf names, use
 *     goodreads_get_user_shelves($user_id, $page, $max_pages)
 * @param string $delete
 *   string containing 'FALSE' if the book should be added from the shelf
 *     or 'TRUE' if the book should be deleted from the shelf
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_add_book_to_shelf($book_id, $shelf_name = '', $delete = '') {
  // Set up parameter array for API call.
  $params = array(
    'book_id' => $book_id,
  );
  if ($shelf_name != '') {
    $params['name'] = $shelf_name;
  }
  if ($delete === 'TRUE') {
    $params['a'] = 'remove';
  }
  // Make the API call.
  $api_result = _goodreads_oauth_post_api('shelf/add_to_shelf.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


/**
 * Update user status using OAuth.
 *
 * This function will update the reading status of the current OAuth user
 * to either xx% complete, on page nnn, or with a general update message.
 *
 * If you are updating a book status ($book_id != ''), you can have either
 * $percentage or $page, but not both.  If you specify both, you will get a
 * ['api_error']=TRUE & result[data][status_message] = 'Unprocessable Entity'
 *
 * You CAN specify both a book status and a general update message in the same
 * status update and the status will reflect both.
 *
 * To get the user statuses use
 *     goodreads_get_user_info($user_id = '', $username = '')
 *     and look in the [updates][update] array
 *
 * @param string $book_id
 *   The Goodreads book ID.
 * @param string $percentage
 *   The percentage of the book that's been read. (optional)
 * @param string $page
 *   The current page number read through. (optional)
 * @param string $message
 *   A general update message
 *   (required, unless page or percent is present, then it is optional)
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_update_user_status($book_id = '', $percentage = '', $page = '', $message = '') {
  // Frankly not sure why I put error checking in this function when the rest
  // don't have it, not that this is much of an error check.
  if ($book_id == '' && $message == '') {
    return array('api_error' => TRUE, 'error_message' => 'must have a book or a message');
  }
  // Set up parameter array for API call.
  $params = array();
  if ($book_id != '') {
    if ($percentage == '' && $page == '') {
      return array('api_error' => TRUE, 'error_message' => 'books must have page or percentage');
    }
    $params['user_status[book_id]'] = $book_id;
  }
  if ($percentage != '') {
    $params['user_status[percent]'] = $percentage;
  }
  if ($page != '') {
    $params['user_status[page]'] = $page;
  }
  if ($message != '') {
    $params['user_status[body]'] = $message;
  }
  // Make the API call.
  $api_result = _goodreads_oauth_post_api('user_status.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


/**
 * Rate a review using OAuth.
 *
 * If you like a review that you have already liked, nothing happens.
 * But if you unlike a review that you haven't already liked, it will
 *   like the review (go figure).
 * Otherwise you can like, then unlike, then like to your heart's content.
 *
 * @param string $resource_id
 *   The Goodreads review ID.
 * @param string $like
 *   'TRUE' if this OAuth user likes the review
 *     or 'FALSE' if this OAuth user 'un-likes' the review
 *   any value other than 'FALSE' will default to 'TRUE'
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_rate_review($resource_id = '', $like = '') {
  // Set up parameter array for API call.
  $params = array(
    'resource_id' => $resource_id,
  );
  if ($like === 'FALSE') {
    $params['rating'] = '0';
  }
  else {
    $params['rating'] = '1';
  }
  // Make the API call.
  $api_result = _goodreads_oauth_post_api('rating/new_review?format=xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result = $result['GoodreadsResponse'];
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


/**
 * Create a comment using OAuth.
 *
 * @param string $resource_id
 *   The Goodreads resource ID.
 * @param string $type
 *   The type of the resource.
 *   one of 'rating', 'poll', 'photo', 'comment', 'book_news_post',
 *     'poll_vote', 'list', 'user_list_challenge', 'user_challenge',
 *     'chapter', 'blog', 'author_blog_post', 'swap_request', 'quiz',
 *     'librarian_note', 'interview', 'topic', 'review', 'fanship',
 *     'user_list_vote', 'recommendation', 'owned_book', 'video',
 *     'user_status', 'question_user_stat', 'question', 'event_response',
 *     'user', 'recommendation_request', 'user_quote', 'quiz_score'
 * @param string $comment
 *   Comment to add.
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_create_comment($resource_id = NULL, $type = '', $comment = '') {
  // Frankly not sure why I put error checking in this function when the
  // rest don't have it, not that this is much of an error check.
  if ($resource_id == '' || $type == '' || $comment == '') {
    return array('api_error' => TRUE, 'error_message' => 'must have all three function parameters');
  }
  // Set up parameter array for API call.
  $params = array(
    'id' => $resource_id,
    'type' => $type,
    'comment[body]' => $comment,
  );
  // Make the API call.
  $api_result = _goodreads_oauth_post_api('comment.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


/**
 * Add to books owned using OAuth.
 *
 * @param string $book_id
 *   The Goodreads book ID. (required)
 * @param string $condition
 *   The Goodreads condition code.  (optional)
 *   one of '10' (brand new), '20' (like new), '30' (very good), '40' (good),
 *     '50' (acceptable), or '60' (poor).
 * @param string $condition_description
 *   Condition description in case the code isn't enough
 *   to keep track of condition details. (optional)
 * @param string $bcid
 *   A BookCrossing id (BCID). (optional)
 *   The Goodreads website says "BookCrossing is a cool site that lets
 *     you track books the other people have left in public places.
 *     Each BookCrossig book has a sticker with a unique ID that you
 *     can use to look it up on their website."  Registering the book is
 *     free.  They have sticker templates you can download and print or
 *     you can buy bookplates (what they call stickers when you pay for them).
 *     Bookcrossing is at http://www.bookcrossing.com
 * @param string $purchase_location
 *   Where this book was purchased. (optional)
 * @param string $purchase_date
 *   When this book was purchased. (optional)
 *   valid date formats???
 *   the date is put through strtotime and converted back to YYYY-MM-DD
 *     to accomodate as wide a range of date formats as possible,
 *     since I know YYYY-MM-DD works
 * @param string $available_for_swap
 *   'true' or 'false' if the book is available for swap.
 *   (optional)
 *   Not sure why this field is still in the API since Goodreads discontinued
 *     book swap effective October 2011.
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 *   Note:  if you try to add a book that has already been added the API
 *     will report success (a "created" status), and it will create DUPLICATE
 *     owned book records! To update an existing owned book, use
 *     goodreads_oauth_update_owned_book($book_id...
 */
function goodreads_oauth_add_owned_book($book_id = '', $condition = '', $condition_description = '', $bcid = '', $purchase_location = '',
$purchase_date = '', $available_for_swap = '') {
  // Set up parameter array for API call.
  $params = array(
    'owned_book[book_id]' => $book_id,
  );
  if ($condition != '') {
    $params['owned_book[condition_code]'] = $condition;
  }
  if ($condition_description != '') {
    $params['owned_book[condition_description]'] = $condition_description;
  }
  if ($bcid != '') {
    $params['owned_book[unique_code]'] = $bcid;
  }
  if ($purchase_location != '') {
    $params['owned_book[original_purchase_location]'] = $purchase_location;
  }
  if ($purchase_date != '') {
    $params['owned_book[original_purchase_date]'] = format_date(strtotime($purchase_date), 'custom', 'Y-d-m');
  }
  if ($available_for_swap != '') {
    $params['owned_book[available_for_swap]'] = $available_for_swap;
  }
  // Make the API call.
  $api_result = _goodreads_oauth_post_api('owned_books.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


/**
 * Update an owned book using OAuth.
 *
 * @param string $owned_book_id
 *   The Goodreads book ID. (required)
 * @param string $condition
 *   The Goodreads condition code.  (optional)
 *   one of '10' (brand new), '20' (like new), '30' (very good), '40' (good),
 *     '50' (acceptable), or '60' (poor).
 * @param string $condition_description
 *   Condition description in case the code isn't enough
 *   to keep track of condition details. (optional)
 * @param string $bcid
 *   BookCrossing id (BCID). (optional)
 *   The Goodreads website says "BookCrossing is a cool site that lets
 *     you track books the other people have left in public places.
 *     Each BookCrossig book has a sticker with a unique ID that you
 *     can use to look it up on their website."  Registering the book is
 *     free.  They have sticker templates you can download and print or
 *     you can buy bookplates (what they call stickers when you pay for them).
 *     Bookcrossing is at http://www.bookcrossing.com
 * @param string $purchase_location
 *   Where this book was purchased. (optional)
 * @param string $purchase_date
 *   When this book was purchased. (optional)
 *   valid date formats???
 *   the date is put through strtotime and converted back to YYYY-MM-DD
 *     to accomodate as wide a range of date formats as possible,
 *     since I know YYYY-MM-DD works
 * @param string $available_for_swap
 *   'true' or 'false' if the book is available for swap. (optional)
 *   Not sure why this field is still in the API since Goodreads discontinued
 *     book swap effective October 2011.
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_oauth_update_owned_book($owned_book_id = '', $condition = '', $condition_description = '', $bcid = '', $purchase_location = '',
$purchase_date = '', $available_for_swap = '') {
  // Set up parameter array for API call.
  $params = array();
  if ($condition != '') {
    $params['owned_book[condition_code]'] = $condition;
  }
  if ($condition_description != '') {
    $params['owned_book[condition_description]'] = $condition_description;
  }
  if ($bcid != '') {
    $params['owned_book[unique_code]'] = $bcid;
  }
  if ($purchase_location != '') {
    $params['owned_book[original_purchase_location]'] = $purchase_location;
  }
  if ($purchase_date != '') {
    $params['owned_book[original_purchase_date]'] = format_date(strtotime($purchase_date), 'custom', 'Y-d-m');
  }
  if ($available_for_swap != '') {
    $params['owned_book[available_for_swap]'] = $available_for_swap;
  }
  // Make the API call.
  $api_result = _goodreads_oauth_put_api('owned_books/' . $owned_book_id . '.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result = $result['GoodreadsResponse'];
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


/**
 * Add a review using OAuth.
 *
 * This function will add a review for a given book for the current OAuth user.
 *
 * To get the review back use
 *     goodreads_get_review_by_user_and_book($user_id = '',
 *       $book_id = '', $include_work = '')
 *
 * @param string $book_id
 *   The Goodreads book ID. (required)
 * @param string $rating
 *   The review rating. (optional_
 *   one of '0', '1', '2', '3', '4', or '5', default is '0' (no rating)
 *   corresponds to the number of stars filled in on the Goodreads site.
 * @param string $review_text
 *   Review text.  (optional)
 * @param string $shelf_name
 *   The name of the shelf to add the book to.
 *   one of 'read', 'currently-reading', 'to-read', or a user's shelf name
 *     which must exist or an error
 *   to get the shelf names, use
 *     goodreads_get_user_shelves($user_id, $page, $max_pages)
 * @param string $date_read
 *   The date the book was read. (optional)
 *   the date is put through strtotime and converted back to YYYY-MM-DD
 *     to accomodate as wide a range of date formats as possible
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_add_review($book_id = '', $rating = '', $review_text = '', $shelf_name = '', $date_read = '') {
  // Set up parameter array for API call.
  $params = array(
    'book_id' => $book_id,
  );
  if ($rating != '') {
    $params['review[rating]'] = $rating;
  }
  if ($review_text != '') {
    $params['review[review]'] = $review_text;
  }
  if ($shelf_name != '') {
    $params['shelf'] = $shelf_name;
  }
  if ($date_read != '') {
    $params['review[read_at]'] = format_date(strtotime($date_read), 'custom', 'Y-d-m');
  }
  // Make the API call.
  $api_result = _goodreads_oauth_post_api('review.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


// ********************* CURRENTLY BROKEN, gets a 401 unauthorized error.
/**
 * Edit a review using OAuth.
 *
 * This function will edit a review for a given book
 * for the current OAuth user.
 *
 * To get the review back use
 *     goodreads_get_review_by_user_and_book($user_id = '',
 *       $book_id = '', $include_work = '')
 *
 * @param string $book_id
 *   The Goodreads book ID.
 * @param string $rating
 *   The review rating. (optional_
 *   one of '0', '1', '2', '3', '4', or '5', default is '0' (no rating)
 *   corresponds to the number of stars filled in on the Goodreads site.
 * @param string $review_text
 *   Review text.  (optional)
 * @param string $shelf_name
 *   The name of the shelf to add the book to.
 *   one of 'read', 'currently-reading', 'to-read', or a user's shelf name
 *     which must exist or an error
 *   to get the shelf names, use
 *     goodreads_get_user_shelves($user_id, $page, $max_pages)
 * @param string $date_read
 *   The date the book was read. (optional)
 *   the date is put through strtotime and converted back to YYYY-MM-DD
 *     to accomodate as wide a range of date formats as possible
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_edit_review($book_id = '', $rating = '', $review_text = '', $shelf_name = '', $date_read = '') {
  // Set up parameter array for API call.
  $params = array(
  // 'book_id' => $book_id,
  // 'format' => 'xml',
  );
  if ($rating != '') {
    $params['review[rating]'] = $rating;
  }
  if ($review_text != '') {
    $params['review[review]'] = $review_text;
  }
  if ($shelf_name != '') {
    $params['shelf'] = $shelf_name;
  }
  if ($date_read != '') {
    $params['review[read_at]'] = format_date(strtotime($date_read), 'custom', 'Y-d-m');
  }
  // Make the API call.
  // $api_result = _goodreads_oauth_put_api('review/edit/' .
  // $book_id . '', $params);
  $api_result = _goodreads_oauth_post_api('review/' . $book_id . '.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


/**
 * Destroy a review using OAuth.
 *
 * This function will add a review for a given book for the current OAuth user.
 *
 * @param string $book_id
 *   The Goodreads book ID.
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_destroy_review($book_id = '') {
  // Set up parameter array for API call.
  $params = array(
    'book_id' => $book_id,
  );
  // Make the API call.
  $api_result = _goodreads_oauth_post_api('review/destroy.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


/**
 * Add a quote to a book using OAuth.
 *
 * @param string $quote
 *   The quote.  (required)
 * @param string $author_name
 *   The author name. (required)
 *   If you don't specify an author_id, it will try to look one up based
 *     on the author_name you provide.
 * @param string $book_id
 *   The Goodreads book ID. (optional)
 *   Note that either $book_id or $isbn MUST be present.
 *   If both are present, the ISBN is ignored and the book ID is used.
 * @param string $isbn
 *   The ISBN of the book. (optional)
 *   Note that either $book_id or $isbn MUST be present.
 *   If both are present, the ISBN is ignored and the book ID is used.
 * @param string $author_id
 *   The Goodreads author ID. (optional)
 *   If you don't specify an author_id, it will try to look one up based
 *     on the author_name you provide.
 * @param string $tags
 *   string containing comma separated tags. (optional)
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 *   Note:  if you try to add a quote that has already been added the API
 *     will report success (a "created" status), and it will return an
 *     quote record with the data passed to the API but the site will
 *     not reflect the information.  Changing the quote, but nothing else
 *     will create a new quote on the site.  But adding a space, even in
 *     the middle of the quote seems to be ignored.
 */
function goodreads_oauth_add_quote($quote = '', $author_name = '', $book_id = '', $isbn = '', $author_id = '', $tags = '') {
  // Frankly not sure why I put error checking in this function when the
  // rest don't have it, not that this is much of an error check.
  if ($book_id == '' && $isbn == '') {
    return array('api_error' => TRUE, 'error_message' => 'must have a book ID or ISBN');
  }
  if ($author_name == '') {
    return array('api_error' => TRUE, 'error_message' => 'must quote author name');
  }
  // Set up parameter array for API call.
  $params = array(
    'quote[body]' => $quote,
    'quote[author_name]' => $author_name,
  );
  if ($book_id != '') {
    $params['quote[book_id]'] = $book_id;
  }
  if ($isbn != '') {
    $params['isbn'] = $isbn;
  }
  if ($author_id != '') {
    $params['quote[author_id]'] = $author_id;
  }
  if ($tags != '') {
    $params['quote[tags]'] = $tags;
  }
  // Make the API call.
  $api_result = _goodreads_oauth_post_api('quotes.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


/**
 * Create a new topic using OAuth.
 *
 * @param string $title
 *   The title for the topic. (required)
 * @param string $subject_type
 *   The subject type for this topic. (required)
 *   Either 'Book' or 'Group'.
 *   If 'Book', the book the topic is about.
 *   If 'Group', the group that the topic belongs to.
 * @param string $subject_id
 *   The id for the subject the topic belongs to, either
 *     book_id or group_id, as appropriate (required)
 * @param string $comment
 *   The text of the comment that starts the topic thread.
 *   (required)
 *   Can include Goodreads book/author tags of the form [book:Title|ID].
 * @param string $update_feed
 *   Whether the comment for the new topic should
 *     be added to the user's update feed.
 *   To enable, set to 'on'; otherwise, default is not to add to update feed.
 * @param string $digest
 *   Whether the user would like to receive an email when
 *     someone replies to the topic (user will get one email only).
 *   To enable, set to 'on'; otherwise, default is not to add to update feed.
 * @param string $folder_id
 *   The Goodreads folder ID if $subject_type = 'Group'
 *   If the subject is a group, you can supply a folder id to add the topic to.
 *   Be sure that the folder belongs to the group.
 *   Default is the topic will be added to the 'general' folder.
 * @param string $question_flag
 *   Whether the topic is a discussion question.
 *   allowed values are '0' = NOT a question or '1' = question
 *     default is '0' (non-question)
 *
 * @return array
 *   array with the status.
 *   return['api_error']=TRUE if something went wrong.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 *   Note:  if you try to add a topic that has already been added the API
 *     will report success (a "created" status), and will duplicate the topic.
 */
function goodreads_oauth_create_topic($title = '', $subject_type = '', $subject_id = '', $comment = '',
$update_feed = '', $digest = '', $folder_id = '', $question_flag = '') {
  // Frankly not sure why I put error checking in this function when the
  // rest don't have it, not that this is much of an error check.
  if ($title == '' && $subject_type == '' && $subject_id == '' && $comment == '') {
    return array('api_error' => TRUE, 'error_message' => 'first 4 parameters are required');
  }
  // Set up parameter array for API call.
  $params = array(
    'topic[title]' => $title,
    'topic[subject_type]' => $subject_type,
    'topic[subject_id]' => $subject_id,
    'comment[body_usertext]' => $comment,
  );
  if (drupal_strtolower($update_feed) == 'on') {
    $params['update_feed'] = drupal_strtolower($update_feed);
  }
  if (drupal_strtolower($digest) == 'on') {
    $params['digest'] = drupal_strtolower($digest);
  }
  if ($folder_id != '') {
    $params['topic[folder_id]'] = $folder_id;
  }
  if ($question_flag != '') {
    $params['topic[question_flag]'] = $question_flag;
  }
  // Make the API call.
  $api_result = _goodreads_oauth_post_api('topic.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_result['api_error'] === TRUE) {
    return $api_result;
  }
  else {
    $result = _goodreads_xml2array($api_result['data']);
    $result = $result['GoodreadsResponse'];
    $result['api_error'] = $api_result['api_error'];
    $result['original_xml'] = $api_result['data'];
    return $result;
  }
}


/**
 * Get a list of the books on a members shelf using OAuth.
 *
 * @param string $user_id
 *   Goodreads user_id. (required by Goodreads)
 *   If missing, current Drupal user's Goodreads ID will be used
 * @param string $sort
 *   How to sort.  (optional)
 *   Options are: available_for_swap, position, num_pages, votes,
 *   recommender, rating, shelves, format, avg_rating, date_pub,
 *   isbn, comments, author, title, notes, cover, isbn13, review,
 *   date_pub_edition, condition, asin, date_started, owned, random,
 *   date_read, year_pub, read_count, date_added, date_purchased,
 *   num_ratings, purchase_location, date_updated.
 * @param string $order
 *   Sort order.  Options are 'a' or 'd' (optional)
 * @param string $shelf_name
 *   The name of the shelf to get the bookbooks from.
 *   defaults to 'read'
 *   to get the shelf names, use
 *     goodreads_get_user_shelves($user_id, $page, $max_pages)
 * @param string $query
 *   Query text to match agains member's books. (optional)
 * @param string $per_page
 *   Number of books to return per page.  1-200 (optional)
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the user's friends will be downloaded.
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or an array with the user's Goodreads information.
 *   return['original_xml'] contains an array of original XML responses,
 *      just in case.
 */
function goodreads_oauth_get_user_books($user_id = '', $sort = '', $order = '', $shelf_name = '',
$query = '', $per_page = '', $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up parameter array for API call.
  $parameters = array(
    // I presume there was an earlier version of the API.
    'v' => '2',
  );
  if ($user_id === '') {
    $user_id = goodreads_oauth_get_user_id();
    $user_id = $user_id['id'];
  }
  if ($sort != '') {
    $parameters['sort'] = $sort;
  }
  if ($order != '') {
    $parameters['order'] = $order;
  }
  if ($shelf_name != '') {
    $parameters['shelf'] = $shelf_name;
  }
  if ($query != '') {
    $parameters['search[query]'] = $query;
  }
  if ($per_page != '') {
    $parameters['per_page'] = $per_page;
  }
  if ($page === '') {
    $parameters['page'] = 1;
    $get_all = TRUE;
  }
  else {
    $parameters['page'] = $page;
    $get_all = FALSE;
  }
  if ($sort != '') {
    $parameters['sort'] = $sort;
  }
  $api_result_array = array();
  $api_xml_result_all = array();
  // Loop through the results.
  do {
    // Make the API call.
    $api_xml_result = _goodreads_oauth_get_api('review/list/' . $user_id . '.xml', $parameters);
    // If something went wrong in the call, return ['api_error'] = TRUE.
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    $api_result = $api_result['GoodreadsResponse']['reviews'];
    $result_count = ((isset($api_result['review'][0])) ? count($api_result['review']) : 0);
    if (($parameters['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result.
      $api_result_array = $api_result;
    }
    else {
      // Otherwise getting additional pages of books,
      // append them to the books array.
      if ($result_count != 0) {
        foreach ($api_result['review'] as $review) {
          $api_result_array['review'][] = $review;
        }
      }
    }
    $parameters['page']++;
    if ($get_all && $parameters['page'] > $max_pages) {
      // After getting $max_pages pages,
      // force the loop to end by setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));

  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * Get a list of all the books on all of a members shelves.
 *
 * This version of the API does use OAuth and viewing members
 * who have set their shelves as visible to members only or just
 * to their friends will not be returned with this function.
 * There is an alternate version NOT using OAuth:
 *   goodreads_get_user_books_all_shelves($user_id....
 *
 * @param string $user_id
 *   Goodreads user_id. (required by Goodreads)
 *   If missing, current Drupal user's Goodreads ID will be used
 * @param string $maximum_books_per_shelf
 *   Maximum number of books to retrieve per shelf.  (optional)
 *   default is 100.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or an array with the user's books.
 *   return['original_xml'] contains an array of original XML responses,
 *      only in the event ['api_error']=TRUE, which causes the function
 *      to return before building the book list.
 */
function goodreads_oauth_get_user_books_all_shelves($user_id, $maximum_books_per_shelf = 100) {
  $books_per_page = floor($maximum_books_per_shelf / 5);
  $shelves = goodreads_get_user_shelves($user_id);
  if ($shelves['api_error'] === TRUE) {
    return $shelves;
  }
  $results == array();
  foreach ($shelves['user_shelf'] as $shelf) {
    $books = goodreads_oauth_get_user_books($user_id, '', '', $shelf['name'], '', $books_per_page);
    if ($books['api_error'] === TRUE) {
      return $books;
    }
    if (isset($books['review'][0])) {
      foreach ($books['review'] as $review) {
        $book = $review['book'];
        $results[] = _goodreads_build_book_result($book, $user_id, $shelf['name']);
      }
    }
    else {
      $book = $books['review']['book'];
      $results[] = _goodreads_build_book_result($book, $user_id, $shelf['name']);
    }
  }
  $results['api_error'] = FALSE;
  return $results;
}


/**
 *   *************************** SUPPORT ROUTINES ***************************
 */


/**
 * Make a Goodreads API call that requires OAuth.
 *
 * @param string $subpath
 *   API subpath to be called.
 * @param array $parameters
 *   Parameters for the API.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or the result object of request.
 */
function _goodreads_oauth_api($subpath, $parameters = array(), $method = 'GET') {
  // If the Goodreads API keys are not set, that's a deal breaker.
  if (!_goodreads_oauth_ready()) {
    return array('api_error' => TRUE, 'error_message' => 'no Goodreads key and secret');
  }
  // Get the access token and secret (which isn't used).
  $result = _goodreads_oauth_get_access_token();
  if ($result === FALSE) {
    return array('api_error' => TRUE, 'error_message' => 'no OAuth access token');
  }
  $access_token = $result->token_key;
  $access_secret = $result->token_secret;
  // Build request URL.
  $data = (count($parameters) > 0) ? drupal_http_build_query($parameters) : NULL;
  $request_url = GOODREADS_ENDPOINT . $subpath;
  // Coder is just plan wrong flagging the next line.
  if (($method == 'GET') || ($method == 'PUT')) {
    $request_url .= '?' . $data;
    $data = NULL;
  }
  // Useful when debugging new API requests.
  // dpm($request_url, 'OAUth ' . $method . ' URL');
  // Set up OAuth and HTTP_client module objects.
  $consumer = DrupalOAuthConsumer::load(variable_get('goodreads_api_key', ''), FALSE);
  $token = new stdClass();
  $token->key = $access_token;
  $token->secret = $access_secret;
  $request = DrupalOAuthRequest::from_consumer_and_token($consumer, $token, $method, $request_url);
  $signature = new OAuthSignatureMethod_HMAC_SHA1();
  $request->sign_request($signature, $consumer, $token);
  // Coder is just plan wrong flagging the next line.
  $header = array(
    'Authorization' => str_replace('Authorization: ', '', $request->to_header()),
  );
  $http_options = array(
    'headers' => $header,
    'method'  => $method,
    'data'    => $data,
    'max_redirects' => GOODREADS_MAX_RETRIES,
    'timeout'       => DEFAULT_SOCKET_TIMEOUT,
  );
  $result = drupal_http_request($request_url, $http_options);
  // dpm($result); // useful when debugging API results.
  $code = floor($result->code / 100) * 100;
  $function_result = array();
  if ($code == 200) {
    $function_result['data'] = $result->data;
    $function_result['api_error'] = FALSE;
    return $function_result;
  }
  else {
    watchdog('Goodreads OAuth', 'Goodreads OAuth call for !path returned response code !code', array('!path' => $subpath, '!code' => $result->code), WATCHDOG_ERROR);
    if ($result->code == HTTP_REQUEST_TIMEOUT) {
      $result->status_message = 'Timeout error communicating with goodreads.com';
    }
    $function_result['data'] = (array) $result;
    $function_result['api_error'] = TRUE;
    $function_result['error_message'] = $result->code . ': ' . (isset($result->status_message) ? $result->status_message : '');
    return $function_result;
  }
}


/**
 * Get the Goodreads OAuth access token/secret for the current user.
 *
 * @return array
 *   FALSE if not present
 *   or object with access token and secret.
 */
function _goodreads_oauth_get_access_token() {
  global $user;
  $userid = $user->uid;
  // Check to see if we should use another user.
  $other_user = &drupal_static('_goodreads_oauth_other_user');
  if (isset($other_user)) {
    $userid = $other_user;
  }
  $anon = ($userid < 1) ? TRUE : FALSE;
  $result = new stdClass();
  if ($anon) {
    // Access token is stored in the session cookie.
    if (!isset($_SESSION['goodreads_oauth']['oauth_token'])) {
      return FALSE;
    }
    $result->token_key = $_SESSION['goodreads_oauth']['oauth_token'];
    $result->token_secret = $_SESSION['goodreads_oauth']['oauth_token_secret'];
  }
  else {
    // Access token is stored in db.
    $db_result = db_select('goodreads_oauth_token', 'got')
      ->condition('uid', $userid, '=')
      ->condition('type', 'access', '=')
      ->fields('got', array('token_key', 'token_secret'))
      ->execute()
      ->fetchAll();
    if (empty($db_result[0])) {
      return FALSE;
    }
    $result->token_key = $db_result[0]->token_key;
    $result->token_secret = $db_result[0]->token_secret;
  }
  return $result;
}


/**
 * Temporarily user another user for OAuth access.
 *
 * Calling this function will change the user ID used for OAuth access
 * from using the currently logged in user to use another Drupal user ID
 * until reset.  This modification only lasts for the current page load,
 * then the Drupal Static variable's lifetime ceases.
 */
function goodreads_oauth_use_another_user($uid) {
  $other_user = &drupal_static('_goodreads_oauth_other_user');
  $other_user = $uid;
}


/**
 * Return to using currently logged in user for OAuth access.
 *
 * Calling this function will change the user ID used for OAuth access
 * back to using the currently logged in user.
 */
function goodreads_oauth_user_current_user() {
  $other_user = &drupal_static('_goodreads_oauth_other_user');
  if (isset($other_user)) {
    drupal_static_reset('_goodreads_oauth_other_user');
  }
}


/**
 * Make a Goodreads API call that requires OAuth and uses GET.
 *
 * @param string $subpath
 *   API subpath to be called.
 * @param array $parameters
 *   Parameters for the API.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or the result object of request.
 */
function _goodreads_oauth_get_api($subpath, $parameters = array()) {
  return _goodreads_oauth_api($subpath, $parameters, 'GET');
}


/**
 * Make a Goodreads API call that requires OAuth and uses POST.
 *
 * @param string $subpath
 *   API subpath to be called.
 * @param array $parameters
 *   Parameters for the API.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or the result object of request.
 */
function _goodreads_oauth_post_api($subpath, $parameters = array()) {
  return _goodreads_oauth_api($subpath, $parameters, 'POST');
}


/**
 * Make a Goodreads API call that requires OAuth and uses DELETE.
 *
 * @param string $subpath
 *   API subpath to be called.
 * @param array $parameters
 *   Parameters for the API.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or the result object of request.
 */
function _goodreads_oauth_delete_api($subpath, $parameters = array()) {
  return _goodreads_oauth_api($subpath, $parameters, 'DELETE');
}


/**
 * Make a Goodreads API call that requires OAuth and uses PUT.
 *
 * @param string $subpath
 *   API subpath to be called.
 * @param array $parameters
 *   Parameters for the API.
 *
 * @return array
 *   return['api_error']=TRUE if something went wrong
 *   or the result object of request.
 */
function _goodreads_oauth_put_api($subpath, $parameters = array()) {
  return _goodreads_oauth_api($subpath, $parameters, 'PUT');
}
