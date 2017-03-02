<?php
/**
 * @file
 * Define the Goodreads Views table schema views
 */

 module_load_include('inc', 'goodreads_views', 'views_fields');


/**
 * Implements hook_views_data().
 */
function goodreads_views_views_data() {
  $data = array(
    // Table definitions.
    'goodreads_views_cache' => array(
      'table' => array(
        'group'                => t("Goodreads Book Data"),
        'base'                 => array(
          'field'              => 'unique_id',
          'title'              => t('Goodreads Book Data'),
          'help'               => t('Goodreads books by user by shelf'),
          'weight'             => 10,
        ),
        // Allow views based on users relate to Goodreads Data.
        'join'                 => array(
          'users'              => array(
            'left_field'       => 'uid',
            'field'            => 'cache_uid',
          ),
        ),
      ),
      // Fields Definitions.
      'unique_id'                => _goodreads_numeric_views_field(t('Unique ID'), t('Unique ID for primary key')),
      'cache_uid'                => _goodreads_numeric_views_field(t('Drupal User ID'), t('User ID from Users.')),
      'cache_time'               => _goodreads_timestamp_views_field(t('Cache Time'), t('Date and time the cache data was created.')),
      'goodreads_user_id'        => _goodreads_text_views_field(t('Goodreads User ID'), t('Goodreads user ID.')),
      'shelf_name'               => _goodreads_text_views_field(t('Shelf Name'), t('Goodreads shelf this book is on.')),
      'title'                    => _goodreads_text_views_field(t('Book Title'), t('Title of book.')),
      'author'                   => _goodreads_text_views_field(t('Author'), t('Primary author of the book.')),
      'ratings_count'            => _goodreads_numeric_views_field(t('Number of Ratings'), t('Number of ratings for this book.')),
      'average_rating'           => _goodreads_numeric_views_field(t('Average Rating'), t('Average rating for this book.')),
      'text_reviews_count'       => _goodreads_numeric_views_field(t('Number of Text Reviews'), t('Number of text reviews.')),
      'image_url'                => _goodreads_text_views_field(t('Book Image URL'), t('URL of a book image.')),
      'isbn'                     => _goodreads_text_views_field(t('ISBN'), t('ISBN for the book.')),
      'goodreads_book_id'        => _goodreads_text_views_field(t('Goodread Book ID'), t('Goodreads book ID for the book.')),
      'description'              => _goodreads_markup_views_field(t('Description'), t('Description of the book.')),
    ),
  );
  // Define a relationship between cache_uid and {users}.uid,
  // allows views based on Goodreads books to relate to users.
  $data['goodreads_views_cache']['cache_uid']['relationship'] = array(
    'handler'                => 'views_handler_relationship',
    'base'                   => 'users',
    'base field'             => 'uid',
    'label'                  => t('User'),
    'relationship field'     => 'cache_uid',
  );

  // dpm($data);
  return $data;

}
