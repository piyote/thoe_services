<?php

/**
 * @file
 * Goodreads Filter Administration Functions
 */

/**
 * Implements hook_settings().
 */
function goodreads_filter_admin_settings() {
  $form['goodreads_filter_function_error'] = array(
    '#type'                  => 'textfield',
    '#title'                 => t('Function not found error message'),
    '#required'              => TRUE,
    '#default_value'         => variable_get('goodreads_filter_function_error', "*** Goodreads function: !function doesn't exist"),
    '#description'           => t("Message you want displayed if the goodreads_function isn't found.  !function will be replaced by the function name."),
  );
  $form['goodreads_filter_api_error'] = array(
    '#type'                  => 'textfield',
    '#title'                 => t('API returned an error message'),
    '#required'              => TRUE,
    '#default_value'         => variable_get('goodreads_filter_api_error', "*** Goodreads function: !function returned an API error: !error"),
    '#description'           => t("Message you want displayed if the goodreads_function isn't found.  !function will be replaced by the function name, !error will be replaced by the error message if one is returned"),
  );

  return system_settings_form($form);
}
