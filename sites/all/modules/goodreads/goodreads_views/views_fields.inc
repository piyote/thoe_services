<?php
/**
 * @file
 * Simplfy defining Drupal views arrays.
 */


/**
 *
 * Functions defined in this file:
 *    _goodreads_boolean_views_field($title, $help = '')
 *    _goodreads_numeric_views_field($title, $help = '')
 *    _goodreads_timestamp_views_field($title, $help = '')
 *    _goodreads_text_views_field($title, $help = '')
 *    _goodreads_markup_views_field($title, $help = '')
 */


if (!function_exists('_goodreads_boolean_views_field')) {
  /**
   * Define a boolean field.
   *
   * @param string $title
   *   The title as it appears in the UI.
   * @param string $help
   *   The help text as it appears in the UI
   *
   * @return array
   *   array to put into views defintion array after the field name like:
   *     'field_name' =>
   *        _boolean_views_field(t('field title'), t('description of field')),
   *   from within an array definition or
   *     $data['table_name']['field_name'] =
   *       _boolean_views_field(t('field title'), t('description of field'));
   */
  function _goodreads_boolean_views_field($title, $help = '') {
    return array(
      // The item it appears as on the UI.
      'title'               => $title,
      // The help that appears on the UI.
      'help'                => $help,
       // Information for displaying a title as a field.
      'field'               => array(
        'handler'           => 'views_handler_field_boolean',
        'click sortable'    => TRUE,
      ),
      'argument'            => array('handler' => 'views_handler_argument_numeric'),
      'filter'              => array(
        'handler'           => 'views_handler_filter_boolean_operator',
        'label'             => $title,
        'type'              => 'yes-no',
      ),
      'sort'                => array('handler' => 'views_handler_sort'),
    );
  }
}


if (!function_exists('_goodreads_numeric_views_field')) {
  /**
   * Define a numeric field.
   *
   * @param string $title
   *   The title as it appears in the UI.
   * @param string $help
   *   The help text as it appears in the UI
   *
   * @return array
   *   array to put into views defintion array after the field name like:
   *     'field_name' =>
   *        _numeric_views_field(t('field title'), t('description of field')),
   *   from within an array definition or
   *     $data['table_name']['field_name'] =
   *       _numeric_views_field(t('field title'), t('description of field'));
   */
  function _goodreads_numeric_views_field($title, $help = '') {
    return array(
      // The item it appears as on the UI.
      'title'               => $title,
      // The help that appears on the UI.
      'help'                => $help,
       // Information for displaying a title as a field.
      'field'               => array(
        'handler'           => 'views_handler_field_numeric',
        'click sortable'    => TRUE,
      ),
      'argument'            => array('handler' => 'views_handler_argument_numeric'),
      'filter'              => array('handler' => 'views_handler_filter_numeric'),
      'sort'                => array('handler' => 'views_handler_sort'),
    );
  }
}


if (!function_exists('_goodreads_timestamp_views_field')) {
  /**
   * Define a timestamp field (aka Unix timestamp as unsigned integer field).
   *
   * For Datetime fields, use the text option, although you won't get the
   * same functionality as a timestamp field.
   *
   * @param string $title
   *   The title as it appears in the UI.
   * @param string $help
   *   The help text as it appears in the UI
   *
   * @return array
   *   array to put into views defintion array after the field name like:
   *     'field_name' =>
   *        _timestamp_views_field(t('field title'), t('description of field')),
   *   from within an array definition or
   *     $data['table_name']['field_name'] =
   *       _timestamp_views_field(t('field title'), t('description of field'));
   */
  function _goodreads_timestamp_views_field($title, $help = '') {
    return array(
      // The item it appears as on the UI.
      'title'               => $title,
      // The help that appears on the UI.
      'help'                => $help,
       // Information for displaying a title as a field.
      'field'               => array(
        'handler'           => 'views_handler_field_date',
        'click sortable'    => TRUE,
      ),
      'argument'            => array('handler' => 'views_handler_argument_date'),
      'filter'              => array('handler' => 'views_handler_filter_date'),
      'sort'                => array('handler' => 'views_handler_date'),
    );
  }
}


if (!function_exists('_goodreads_text_views_field')) {
  /**
   * Define a text field.
   *
   * @param string $title
   *   The title as it appears in the UI.
   * @param string $help
   *   The help text as it appears in the UI
   *
   * @return array
   *   array to put into views defintion array after the field name like:
   *     'field_name' =>
   *        _text_views_field(t('field title'), t('description of field')),
   *   from within an array definition or
   *     $data['table_name']['field_name'] =
   *       _text_views_field(t('field title'), t('description of field'));
   */
  function _goodreads_text_views_field($title, $help = '') {
    return array(
      // The item it appears as on the UI.
      'title'               => $title,
      // The help that appears on the UI.
      'help'                => $help,
       // Information for displaying a title as a field.
      'field'               => array(
        'handler'           => 'views_handler_field',
        'click sortable'    => TRUE,
      ),
      'argument'            => array('handler' => 'views_handler_argument_string'),
      'filter'              => array('handler' => 'views_handler_filter_string'),
      'sort'                => array('handler' => 'views_handler_sort'),
    );
  }
}


if (!function_exists('_goodreads_markup_views_field')) {
  /**
   * Define a markup field, a text field containing HTML markup.
   *
   * @param string $title
   *   The title as it appears in the UI.
   * @param string $help
   *   The help text as it appears in the UI
   *
   * @return array
   *   array to put into views defintion array after the field name like:
   *     'field_name' =>
   *        _markup_views_field(t('field title'), t('description of field')),
   *   from within an array definition or
   *     $data['table_name']['field_name'] =
   *       _markup_views_field(t('field title'), t('description of field'));
   */
  function _goodreads_markup_views_field($title, $help = '') {
    return array(
      // The item it appears as on the UI.
      'title'               => $title,
      // The help that appears on the UI.
      'help'                => $help,
       // Information for displaying a title as a field.
      'field'               => array(
        'handler'           => 'views_handler_field_markup',
        'format'            => filter_fallback_format(),
      ),
      // 'argument' => array('handler' => 'can't have markup as an argument'),
      // 'filter'   => array('handler' => 'can't filter by markup'),
      // 'sort'     => array('handler' => 'can't sort markup'),
    );
  }
}
