<?php

/**
 * @file
 * Goodreads OAuth Administration Functions
 *
 * @todo Need to review this whole file,
 *   may have extraneous stuff or broken links
 */


/**
 * Implements hook_settings().
 */
function goodreads_oauth_settings() {
  $form['goodreads_api_key'] = array(
    '#type'                  => 'textfield',
    '#title'                 => t('API key'),
    '#required'              => TRUE,
    '#default_value'         => variable_get('goodreads_api_key', ''),
    '#description'           => t('API key from Goodreads'),
  );
  $form['goodreads_api_secret'] = array(
    '#type'                  => 'textfield',
    '#title'                 => t('API secret'),
    '#required'              => TRUE,
    '#default_value'         => variable_get('goodreads_api_secret', ''),
    '#description'           => t("API key's secret from Goodreads."),
  );
  // Add a submit handler before the system_settings_form submit handler.
  $form['#submit'] = array('goodreads_admin_settings_submit');

  return system_settings_form($form);
}


/**
 * Submit handler for goodreads_oauth_settings.
 */
function goodreads_oauth_settings_submit($form, &$form_state) {
  // Clean up the data ...
  $form_state['values']['goodreads_api_key'] = trim($form_state['values']['goodreads_api_key']);
  $form_state['values']['goodreads_api_secret'] = trim($form_state['values']['goodreads_api_secret']);
}


/**
 * Callback for goodreads related user settings at user/%/edit/goodreads.
 */
function goodreads_oauth_user_settings($account) {
  $output = '';
  if (!_goodreads_oauth_ready()) {
    $output .= t('The Goodreads API keys have not been set');
    $output .= drupal_render(drupal_get_form('goodreads_admin_settings'));
  }
  else {
    if (!_goodreads_oauth_get_access_token()) {
      $output .= t('You must first authorize integration with Goodreads to use related features.');
      $output .= drupal_render(drupal_get_form('goodreads_user_enable_form', $account->uid));
    }
    else {
      $output .= t('Your account is associated with a Goodreads profile.');
      $output .= drupal_render(drupal_get_form('goodreads_user_settings_form', $account));
    }
  }
  return $output;
}


/**
 * Take users to Goodreads auth page.
 */
function goodreads_user_enable_form($form_state, $uid) {
  $form = array();
  $form['#user'] = $uid;
  // $form['#action'] = url('goodreads/token/' . $uid);
  $form['submit'] = array(
    '#type' => 'submit',
     // I disagree with Coder review on this.
    '#value' => t('Go to Goodreads.'),
    '#suffix' => '<p class="description">' . t('You will be taken to goodreads website in order to complete the process.') . '</p>',
  );
  return $form;
}

/**
 * Submit handler for Goodreads auth page.
 */
function goodreads_user_enable_form_submit($form, &$form_state) {
  goodreads_oauth_request_access_token();
}


/**
 * Form for individual user settings.
 */
function goodreads_user_settings_form($form_state, $account) {
  // Just fetch forms from submodules.
  $form = module_invoke_all('goodreads_user_settings_page', $form_state, $account);
  // We will need the account at submit.
  $form['#account'] = $account;
  $form['submit'] = array(
    '#type'                => 'submit',
    '#value'               => 'Save',
  );
  $form['goodreads']['reset'] = array(
    '#type'                => 'submit',
    '#description'         => t('Click here to unlink your goodreads account.'),
    '#value'               => t('Unlink'),
  );
  return $form;
}

/**
 * Submit handler for individual user settings.
 */
function goodreads_user_settings_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];

  if ($op == $form['goodreads']['reset']['#value']) {
    foreach ($form_state['values'] as $key => $val) {
      if (drupal_substr($key, 0, 8) == 'goodreads') {
        user_save($form['#account'], array($key => NULL), 'goodreads');
      }
    }
    db_query("DELETE FROM {goodreads_oauth_token} WHERE uid = %d", $form['#account']->uid);
    drupal_set_message(t('goodreads preferences have been reset'));
  }
  else {
    foreach ($form_state['values'] as $key => $val) {
      if (drupal_substr($key, 0, 8) == 'goodreads') {
        user_save($form['#account'], array($key => $val), 'goodreads');
      }
    }
    drupal_set_message(t('goodreads preferences have been saved'));
  }
}
