<?php

/**
 * @file
 * Goodreads Administration Functions
 */


/**
 * Implements hook_settings().
 */
function goodreads_admin_settings() {
  $form['goodreads_api_key'] = array(
    '#type'                  => 'textfield',
    '#title'                 => t('API key'),
    '#required'              => TRUE,
    '#default_value'         => variable_get('goodreads_api_key', ''),
    '#description'           => l(t('API key from Goodreads'), 'http://www.goodreads.com/api/keys', array('attributes' => array('target' => '_blank'))),
  );
  $form['goodreads_api_secret'] = array(
    '#type'                  => 'textfield',
    '#title'                 => t('API secret'),
    '#required'              => TRUE,
    '#default_value'         => variable_get('goodreads_api_secret', ''),
    '#description'           => t("API key's secret from Goodreads."),
  );
  $lifetime_options = array(0 => t('No cacheing'));
  $options = array(3600, 7200, 14400, 21600, 43200, 86400);
  $lifetime_options += drupal_map_assoc($options, 'format_interval');
  $form['goodreads_cache_lifetime'] = array(
    '#type'                  => 'select',
    '#title'                 => t('Goodreads cache lifetime'),
    '#description'           => t('Cached information from Goodreads must be refreshed regularly, yet refreshing it too often can cause the site to make too many API requests. How long should a request be cached until a new request is made.'),
    '#default_value'         => variable_get('goodreads_cache_lifetime', 86400),
    '#options'               => $lifetime_options,
  );
  $form['clear_cache'] = array(
    '#type'                  => 'fieldset',
    '#title'                 => t('Clear Goodreads cache'),
    '#description'           => t('Caching data improves performance, but may cause problems while troubleshooting new modules, themes, or translations, if outdated information has been cached. To refresh all Goodreads cached data on your site, click the button below.'),
  );

  $form['clear_cache']['clear'] = array(
    '#type'                  => 'submit',
    '#value'                 => t('Clear Goodreads cache'),
    '#submit'                => array('goodreads_clear_cache_submit'),
  );
  // Add a submit handler before the system_settings_form submit handler.
  $form['#submit'] = array('goodreads_admin_settings_submit');
  return system_settings_form($form);
}


/**
 * Submit handler for goodreads_admin_settings.
 */
function goodreads_admin_settings_submit($form, &$form_state) {
  // Clean up the data ...
  $form_state['values']['goodreads_api_key'] = trim($form_state['values']['goodreads_api_key']);
  $form_state['values']['goodreads_api_secret'] = trim($form_state['values']['goodreads_api_secret']);
}


/**
 * Submit callback; clear Goodreads cache.
 */
function goodreads_clear_cache_submit($form, &$form_state) {
  goodreads_cache_clear();
  drupal_set_message(t('Goodreads cache cleared.'));
}


/**
 * Clear the Goodreads cache table.
 */
function goodreads_cache_clear() {
  db_query('TRUNCATE TABLE {goodreads_cache}');
}


/**
 * Form to allow debugging the actual retrieval from goodreads.
 */
function goodreads_test_form($form, &$form_state) {
  $form = array();

  $form['isbn'] = array(
    '#type'                  => 'textfield',
    '#title'                 => t('ISBN'),
    '#description'           => t('ISBN-10, or ISBN-13 of a book listed on goodreads.'),
    '#required'              => TRUE,
    '#default_value'         => '1430209895',
  );
  $form['submit'] = array(
    '#type'                  => 'submit',
    '#value'                 => t('Look up book'),
  );

  if (isset($form_state['goodreads_item'])) {
    $form['item_data'] = array(
      '#type' => 'fieldset',
      '#title' => t('Result'),
      '#collapsible' => FALSE,
    );
    $form['item_data']['title'] = array(
      '#markup'              => '<h2>' . check_plain($form_state['goodreads_item']['title']) . '</h2>',
      '#weight'             => 2,
    );
    $markup = $form_state['goodreads_item']['reviews_widget'] . '<br />';
    $form['item_data']['display'] = array(
      '#markup'              => filter_xss($markup, array('iframe', 'style')),
      '#weight'             => 5,
    );
    $form['item_data']['details'] = array(
      '#type' => 'fieldset',
      '#title' => t('Details'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 10,
    );
    $form['item_data']['details']['data'] = array(
      '#markup' => '<pre><small>' . print_r($form_state['goodreads_item'], TRUE) . '</small></pre>',
    );
  }
  return $form;
}

/**
 * Test form validate handler.
 */
function goodreads_test_form_validate($form, &$form_state) {
  $isbn = $form_state['values']['isbn'];

  $item = goodreads_get_isbn_reviews($isbn);
  if (!$item['api_error']) {
    $form_state['goodreads_item'] = $item;
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message(t('Test failed for this ISBN. Please check the error log for messages'));
  }
}

/**
 * Test form submit handler.
 */
function goodreads_test_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}
