<?php
/**
 * @file
 * Simplfy defining Drupal schema arrays.
 */


/**
 * Functions defined in this file:
 *   _goodreads_oauth_int_field($description = 'No description given',
 *      $unsigned = TRUE, $not_null = TRUE)
 *   _goodreads_oauth_bigint_field($description = 'No description given',
 *      $unsigned = TRUE, $not_null = TRUE)
 *   _goodreads_oauth_serial_field($description = 'No description given',
 *      $unsigned = TRUE, $not_null = TRUE)
 *   _goodreads_oauth_float_field($description = 'No description given',
 *      $unsigned = TRUE, $not_null = TRUE)
 *   _goodreads_oauth_numeric_field($description = 'No description given',
 *      $precision = 8, $scale = 2, $unsigned = TRUE, $not_null = TRUE)
 *   _goodreads_oauth_text_field($description = 'No description given',
 *      $not_null = TRUE)
 *   _goodreads_oauth_bigtext_field($description = 'No description given',
 *      $not_null = TRUE)
 *   _goodreads_oauth_varchar_field($description = 'No description given',
 *      $length = 250, $not_null = TRUE
 *   _goodreads_oauth_char_field($description = 'No description given',
 *      $length = 250, $not_null = TRUE
 *   _goodreads_oauth_datetime_field($description = 'No description given',
 *      $not_null = TRUE)
 *   _goodreads_oauth_timestamp_field($description = 'No description given',
 *      $not_null = TRUE)
 */


/**
 * Define an integer database field.
 *
 * @param string $description
 *   The description of the field
 * @param boolean $unsigned
 *   boolean = TRUE if field is to be unsigned, default TRUE
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _int_field(t('description of field')),
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _int_field(t('description of field'));
 */
function _goodreads_oauth_int_field($description = 'No description given', $unsigned = TRUE, $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'int',
    'unsigned'    => $unsigned,
    'not null'    => $not_null,
  );
}


/**
 * Define a big integer database field.
 *
 * @param string $description
 *   The description of the field
 * @param boolean $unsigned
 *   boolean = TRUE if field is to be unsigned, default TRUE
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _bigint_field(t('description of field')),
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _bigint_field(t('description of field'));
 */
function _goodreads_oauth_bigint_field($description = 'No description given', $unsigned = TRUE, $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'int',
    'size'        => 'big',
    'unsigned'    => $unsigned,
    'not null'    => $not_null,
  );
}


/**
 * Define a serial database field.
 *
 * @param string $description
 *   The description of the field
 * @param boolean $unsigned
 *   boolean = TRUE if field is to be unsigned, default TRUE
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _serial_field(t('description of field')),
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _serial_field(t('description of field'));
 */
function _goodreads_oauth_serial_field($description = 'No description given', $unsigned = TRUE, $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'serial',
    'unsigned'    => $unsigned,
    'not null'    => $not_null,
  );
}


/**
 * Define a float database field.
 *
 * @param string $description
 *   The description of the field
 * @param boolean $unsigned
 *   boolean = TRUE if field is to be unsigned, default TRUE
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _float_field(t('description of field')),
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _float_field(t('description of field'));
 */
function _goodreads_oauth_float_field($description = 'No description given', $unsigned = TRUE, $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'float',
    'unsigned'    => $unsigned,
    'not null'    => $not_null,
  );
}


/**
 * Define a numeric (fixed precision) database field.
 *
 * @param string $description
 *   The description of the field.
 * @param integer $precision
 *   number of TOTAL digits.
 *   number of digits to the LEFT of the decimal point = $precision - $scale.
 * @param integer $scale
 *   number of digits to the RIGHT of the decimal point.
 * @param boolean $unsigned
 *   boolean = TRUE if field is to be unsigned, default TRUE
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _numeric_field(t('description of field'), 8, 2),
 *   will define field that holds 123456.12
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _numeric_field(t('description of field'), 8, 2);
 */
function _goodreads_oauth_numeric_field($description = 'No description given', $precision = 8, $scale = 2, $unsigned = TRUE, $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'numeric',
    'precision'   => $precision,
    'scale'       => $scale,
    'unsigned'    => $unsigned,
    'not null'    => $not_null,
  );
}


/**
 * Define a text database field.
 *
 * @param string $description
 *   The description of the field
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _text_field(t('description of field')),
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _text_field(t('description of field'));
 */
function _goodreads_oauth_text_field($description = 'No description given', $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'text',
    'not null'    => $not_null,
  );
}


/**
 * Define a big text database field.
 *
 * @param string $description
 *   The description of the field
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _bigtext_field(t('description of field')),
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _bigtext_field(t('description of field'));
 */
function _goodreads_oauth_bigtext_field($description = 'No description given', $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'text',
    'size'        => 'big',
    'not null'    => $not_null,
  );
}


/**
 * Define a varchar database field.
 *
 * @param string $description
 *   The description of the field
 * @param integer $length
 *   length of the field
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _varchar_field(t('description of field', 250)),
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _varchar_field(t('description of field'), 250);
 */
function _goodreads_oauth_varchar_field($description = 'No description given', $length = 250, $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'varchar',
    'length'      => $length,
    'not null'    => $not_null,
  );
}


/**
 * Define a char (fixed length) database field.
 *
 * @param string $description
 *   The description of the field
 * @param integer $length
 *   length of the field
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _char_field(t('description of field', 250)),
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _char_field(t('description of field', 250));
 */
function _goodreads_oauth_char_field($description = 'No description given', $length = 250, $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'char',
    'length'      => $length,
    'not null'    => $not_null,
  );
}


/**
 * Define a datetime (stored as text) database field.
 *
 * @param string $description
 *   The description of the field
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _datetime_field(t('description of field', 250)),
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _datetime_field(t('description of field', 250));
 */
function _goodreads_oauth_datetime_field($description = 'No description given', $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'datetime',
    'not null'    => $not_null,
  );
}


/**
 * Define a timestamp database field.
 *
 * @param string $description
 *   The description of the field
 * @param boolean $not_null
 *   boolean = TRUE if field is not to be NULL, default TRUE
 *
 * @return array
 *   array to put into scheme after the field name like:
 *   'fieldname' => _timestamp_field(t('description of field')),
 *   from within an array definition or
 *   $schema['table_name']['fields']['fieldname'] =
 *     _timestamp_field(t('description of field'));
 */
function _goodreads_oauth_timestamp_field($description = 'No description given', $not_null = TRUE) {
  return array(
    'description' => $description,
    'type'        => 'int',
    'size'        => 'big',
    'unsigned'    => TRUE,
    'not null'    => $not_null,
  );
}
