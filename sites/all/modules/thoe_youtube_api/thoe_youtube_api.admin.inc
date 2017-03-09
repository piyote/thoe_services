<?php

/**
 * @file
 * Admin forms and callbacks for youtube api module.
 */

/**
 * General settings form.
 */
function thoe_youtube_api_settings_form($form, &$form_state) {
  $form['web_application'] = array(
    '#type' => 'fieldset',
    '#title' => t('Web application setup'),
    '#description' => t('These credentials are only for web application use.'),
    '#collapsible' => TRUE,
  );

  $form['web_application']['thoe_youtube_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('API key'),
    '#default_value' => variable_get('thoe_youtube_api_key', ''),
  );

  return system_settings_form($form);
}
