<?php

/**
 * @file
 * Goodreads Views Administration Functions
 */

/**
 * Implements hook_settings().
 */
function goodreads_views_admin_settings() {
  $form = array();
  $form['goodreads_views_max_books_per_shelf'] = array(
    '#type'                  => 'textfield',
    '#title'                 => t('Maximum number of books per shelf'),
    '#required'              => TRUE,
    '#size'                  => 10,
    '#default_value'         => variable_get('goodreads_views_max_books_per_shelf', 100),
    '#description'           => t("Enter the maximum number of books you want to retrieve per shelf.  Because of the way it is implemented, the value here will be rounded down to the nearest multiple of 5"),
  );
  for ($i = 0; $i <= 23; $i++) {
    $hour_options[$i] = $i . ':00';
  }
  $form['goodreads_views_cron_hour'] = array(
    '#type'                  => 'select',
    '#title'                 => t('Goodreads views cron scheduled time'),
    '#required'              => TRUE,
    '#default_value'         => variable_get('goodreads_views_cron_hour', "23"),
    '#options'               => $hour_options,
    '#description'           => t("What hour on a 24 hour clock you want the goodreads views cron to run."),
  );
  $form['force_queue'] = array(
    '#type'                  => 'fieldset',
    '#title'                 => t('Force Goodreads views cache fill'),
    '#description'           => t('Normally the Goodreads views cache is filled at the time listed above, using Drupal Queue to avoid cron timeouts. To force the data to be recreated at any time, click the button below.  Then you might need to manually run cron one or more times.'),
  );

  $form['force_queue']['clear'] = array(
    '#type'                  => 'submit',
    '#value'                 => t('Force Goodreads views cache fill'),
    '#submit'                => array('goodreads_views_force_queue'),
  );
  $form['clear_cache'] = array(
    '#type'                  => 'fieldset',
    '#title'                 => t('Clear Goodreads views cache'),
    '#description'           => t('Caching data improves performance, but may cause problems while troubleshooting new modules, themes, or translations, if outdated information has been cached. To refresh all Goodreads views cached data on your site, click the button below.'),
  );

  $form['clear_cache']['clear'] = array(
    '#type'                  => 'submit',
    '#value'                 => t('Clear Goodreads views cache'),
    '#submit'                => array('goodreads_views_clear_cache_submit'),
  );

  return system_settings_form($form);
}


/**
 * Submit callback: force queueing views data.
 */
function goodreads_views_force_queue($form, &$form_state) {
  _goodreads_views_cron_fill_queue();
  drupal_set_message(t('Goodreads views data queue created.  You may need to run cron one or more times to import the data'));
}


/**
 * Submit callback: clear Goodreads cache.
 */
function goodreads_views_clear_cache_submit($form, &$form_state) {
  goodreads_views_cache_clear();
  drupal_set_message(t('Goodreads views cache cleared.'));
}


/**
 * Clear the Goodreads views cache table.
 */
function goodreads_views_cache_clear() {
  db_query('TRUNCATE TABLE {goodreads_views_cache}');
}
