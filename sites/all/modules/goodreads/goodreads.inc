<?php

/**
 * @file
 * Supporting routines for Goodreads.
 */


/**
 * API functions in this file:
 *   goodreads_get_gid_reviews($gid, $rating = '', $text_only = '')
 *   goodreads_get_isbn_reviews($isbn, $rating = '', $text_only = '')
 *   goodreads_get_reviews_title($title, $author = '', $rating = '',
 *     $text_only = '')
 *   goodreads_get_review($review_id, $page = '',
 *     $max_pages = GOODREADS_MAX_PAGES)
 *   goodreads_get_review_by_user_and_book($user_id = '',
 *     $book_id = '', $include_work = '')
 *   goodreads_get_recent_reviews()
 *   goodreads_get_isbn_review_statistics($isbn) **
 *   goodreads_isbn2id($isbn)
 *   goodreads_book_search($q, $field = '', $page = '',
 *     $max_pages = GOODREADS_MAX_PAGES)
 *   goodreads_get_author_book_list($author_id, $page = '',
 *     $max_pages = GOODREADS_MAX_PAGES)
 *   goodreads_get_author_info($author_id)
 *   goodreads_find_author($author_name)
 *   goodreads_get_author_series($author_id)
 *   goodreads_get_series_info($series_id)
 *   goodreads_get_work_series($work_id)
 *   goodreads_get_group_by_id($group_id)
 *   goodreads_get_group_members($group_id, $page = '', $sort = '',
 *     $q = '', $max_pages = GOODREADS_MAX_PAGES)
 *   goodreads_get_user_groups($user_id)
 *   goodreads_get_user_info($user_id = '', $username = '')
 *   goodreads_get_recent_user_status()
 *   goodreads_get_user_follower_list($user_id, $page = '',
 *     $max_pages = GOODREADS_MAX_PAGES) **
 *   goodreads_get_user_following_list($user_id, $page = '',
 *     $max_pages = GOODREADS_MAX_PAGES) **
 *   goodreads_get_user_shelves($user_id, $page = '',
 *     $max_pages = GOODREADS_MAX_PAGES)
 *   goodreads_get_user_books($user_id = '', $sort = '', $order = '',
 *     $shelf_name = '', $query = '', $per_page = '', $page = '',
 *     $max_pages = GOODREADS_MAX_PAGES)
 *   goodreads_get_user_books_all_shelves($user_id,
 *     $maximum_books_per_shelf = 100)
 *   goodreads_get_topics_by_group_and_folder($group_id = '',
 *     $folder_id = '', $sort = '', $order = '', $page = '',
 *     $max_pages = GOODREADS_MAX_PAGES)
 *   goodreads_get_topic_info($topic_id = '')
 *   goodreads_get_comments($id = '', $type = '', $page = '',
 *     $max_pages = GOODREADS_MAX_PAGES)
 *   goodreads_get_events($lat = '', $lng = '') **
 *   goodreads_cache_disable()
 *   goodreads_cache_enable()
 *
 * ** indicates there is some serious issue with the API call,
 *    see function definition for details.
 */


/**
 * Get the Goodreads reviews for a Goodreads book ID.
 *
 * @param string $gid
 *   The Goodreads book ID to get reviews for.
 * @param string $rating
 *   Show only reviews with a particular rating.
 *   Options are: 1, 2, 3, 4, or 5 (optional)
 * @param string $text_only
 *   Only show reviews that have text. (default false)
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_gid_reviews($gid, $rating = '', $text_only = '') {
  // Set up the parameter array for the API call..
  $params = array(
    'id' => $gid,
    'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($rating != '') {
    $params['rating'] = $rating;
  }
  if ($text_only != '') {
    $params['text_only'] = $text_only;
  }
  // MMake the API call..
  $api_xml_result = _goodreads_request('book/show', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE..
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['book'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * Get the Goodreads reviews for a ISBN.
 *
 * @param string $isbn
 *   The ISBN to get reviews for.
 * @param string $rating
 *   Show only reviews with a particular rating.
 *   Options are: 1, 2, 3, 4, or 5 (optional).
 * @param string $text_only
 *   Only show reviews that have text. (default false)
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_isbn_reviews($isbn, $rating = '', $text_only = '') {
  // Set up the parameter array for the API call..
  $params = array(
    'isbn' => $isbn,
    'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($rating != '') {
    $params['rating'] = $rating;
  }
  if ($text_only != '') {
    $params['text_only'] = $text_only;
  }
  // Make the API call..
  $api_xml_result = _goodreads_request('book/isbn', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE..
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['book'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * Get the Goodreads reviews for a specific title.
 *
 * @param string $title
 *   The book title to get reviews for.
 *   Spaces will be converted to plus signs by this function.
 * @param string $author
 *   The book title to get reviews for. (optional)
 *   Spaces will be converted to plus signs by this function.
 * @param string $rating
 *   Show only reviews with a particular rating.
 *   Options are: 1, 2, 3, 4, or 5 (optional)
 * @param string $text_only
 *   Only show reviews that have text. (default false)
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_reviews_title($title, $author = '', $rating = '', $text_only = '') {
  // Set up the parameter array for the API call..
  // Replace spaces with plus signs in title.
  $title = urlencode($title);
  $params = array(
    'title' => $title,
    'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($author != '') {
    // Replace spaces with plus signs in author's name.
    $author = urlencode($author);
    $params['author'] = $author;
  }
  if ($rating != '') {
    $params['rating'] = $rating;
  }
  if ($text_only != '') {
    $params['text_only'] = $text_only;
  }
  // Make the API call..
  $api_xml_result = _goodreads_request('book/title', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE..
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['book'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * Get a review.
 *
 * @param string $review_id
 *   The Goodreads review ID.
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the comments will be downloaded.
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *     and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_review($review_id, $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up the parameter array for the API call..
  $params = array(
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
  // Loop through the results..
  do {
    // Make the API call..
    $api_xml_result = _goodreads_request('review/show/' . $review_id . '.xml', $params);
    // If something went wrong in the call, return ['api_error'] = TRUE..
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    $api_result = $api_result['GoodreadsResponse']['review'];
    $result_count = isset($api_result['comments']['comment']) ? count($api_result['comments']['comment']) : 0;
    if (($params['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result..
      $api_result_array = $api_result;
    }
    else {
      // Otherwise getting additional pages of comments,
      // append them to the comments array.
      if ($result_count != 0) {
        foreach ($api_result['comments']['comment'] as $comment) {
          $api_result_array['comments']['comment'][] = $comment;
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


/**
 * Get a user's review for a given book.
 *
 * @param string $user_id
 *   The Goodreads user ID.
 * @param string $book_id
 *   The the Goodreads book ID to get reviews for.
 * @param string $include_work
 *   A string to indicate whether to return a review for another book in the
 *   same work if review not found for the specified book
 *   'true' or 'false' , default 'false'. (optional)
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_review_by_user_and_book($user_id = '', $book_id = '', $include_work = '') {
  // Set up the parameter array for the API call..
  $params = array(
    // 'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($user_id != '') {
    $params['user_id'] = $user_id;
  }
  if ($book_id != '') {
    $params['book_id'] = $book_id;
  }
  if ($include_work != '') {
    $params['username'] = $include_work;
  }
  // Make the API call..
  $api_xml_result = _goodreads_request('review/show_by_user_and_book.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE..
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  if ($api_xml_result == 'review not found for user') {
    $api_result = array('not found' => 'true');
  }
  else {
    $api_result = $api_result['GoodreadsResponse']['review'];
  }
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * Recent reviews from all members.
 *
 * Returns the 20 most recently added reviews from all members.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_recent_reviews() {
  // Set up the parameter array for the API call.
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call.
  $api_xml_result = _goodreads_request('review/recent_reviews.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['reviews'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


// ************* Only appears to work for a single ISBN.
/**
 * Get the Goodreads review statistics for multiple ISBNs.
 *
 * @param string $isbn
 *   The ISBN to get reviews for.
 *   More than one can be queried at once, separate them with commas.
 *     But do not include any spaces or other non-numeric characters!!
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_isbn_review_statistics($isbn) {
  // Encode any commas in the ISBN string.
  $isbn = str_replace(' ', '', $isbn);
  $isbn = urlencode($isbn);
  // Set up the parameter array for the API call..
  $params = array(
    'isbns' => $isbn,
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call..
  // This API call may only have a JSON response, so use JSON.
  $api_json_result = _goodreads_request('book/review_counts.json', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE..
  if ($api_json_result['api_error'] === TRUE) {
    return $api_json_result;
  }
  else {
    $api_json_result = $api_json_result['data'];
  }
  $api_result = (array) json_decode($api_json_result);
  foreach ($api_result['books'] as $key => $value) {
    $api_result['books'][$key] = (array) $value;
  }
  $api_result['original_xml'] = $api_json_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * Convert an ISBN into a Goodreads book ID.
 *
 * @param string $isbn
 *   The ISBN to get reviews for.
 *
 * @return array
 *   Array with ['id'] = Goodreads book ID.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_isbn2id($isbn) {
  // Set up the parameter array for the API call..
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call..
  $api_xml_result = _goodreads_request('book/isbn_to_id/' . $isbn, $params);
  // If something went wrong in the call, return ['api_error'] = TRUE..
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  return array(
    'id' => $api_xml_result,
    'api_error' => FALSE,
    'original_xml' => $api_xml_result,
  );
}


// ************* Number of results returned != number said returned.
/**
 * Search books.
 *
 * This will search all books in the title/author/ISBN fields and show
 * matches, sorted by popularity on Goodreads. There will be cases where
 * a result is shown on the Goodreads site, but not through the API.
 * This happens when the result is an Amazon-only edition and they have
 * to honor Amazon's terms of service.
 *
 * To search by ISBN put "isbn" as search field
 *  and your isbn value as "q" param.
 *
 * @param string $q
 *   The query text to match against book title,
 *     author, and ISBN fields.
 *   Supports boolean operators and phrase searching.
 * @param string $field
 *   Which field to search, one of title, author,
 *     genre, isbn, or all (default is all).
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the books will be downloaded.
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_book_search($q, $field = '', $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up the parameter array for the API call..
  $params = array(
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
  if ($field != '') {
    $params['search%5bfield%5d'] = $field;
  }
  if ($q != '') {
    // Encode query to pass via URL.
    $q = urlencode($q);
    $params['q'] = $q;
  }
  $api_result_array = array();
  $api_xml_result_all = array();
  // Loop through the results..
  do {
    // Make the API call..
    $api_xml_result = _goodreads_request('search/index.xml', $params);
    // If something went wrong in the call, return ['api_error'] = TRUE..
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    $api_result = $api_result['GoodreadsResponse']['search'];
    $result_count = isset($api_result['results']['work']) ? count($api_result['results']['work']) : 0;
    if ($api_result['total-results'] == 1) {
      // Only one result, don't make any more calls to API,
      // which returns odd results.
      $result_count = 0;
    }
    if (($params['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result..
      $api_result_array = $api_result;
    }
    else {
      // Otherwise getting additional pages of books,
      // append them to the books array.
      if ($result_count != 0) {
        foreach ($api_result['results']['work'] as $book) {
          $api_result_array['results']['work'][] = $book;
        }
      }
    }
    $params['page']++;
    if ($get_all && $params['page'] > $max_pages) {
      // After getting $max_pages pages, force the loop to end
      // by setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));

  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * Get a list of an author's books.
 *
 * @param string $author_id
 *   The Goodreads author ID.
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the books will be downloaded.
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_author_book_list($author_id, $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up the parameter array for the API call.
  $params = array(
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
  // Loop through the results..
  do {
    // Make the API call.
    $api_xml_result = _goodreads_request('author/list/' . $author_id . '.xml', $params);
    // If something went wrong in the call, return ['api_error'] = TRUE.
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    $api_result = $api_result['GoodreadsResponse']['author'];
    $result_count = isset($api_result['books']['book']) ? count($api_result['books']['book']) : 0;
    if (($params['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result..
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
      // After getting $max_pages pages, force the loop to end
      // by setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));
  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * Get an author's information.
 *
 * @param string $author_id
 *   The Goodreads author ID.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_author_info($author_id) {
  // Set up the parameter array for the API call.
  $params = array(
    'id' => $author_id,
    'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call.
  $api_xml_result = _goodreads_request('author/show/', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['author'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * Find an author by name.
 *
 * @param string $author_name
 *   The author'a name.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   No data is returned, yet ['api_error']==FALSE if the author isn't found.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_find_author($author_name) {
  // Set up the parameter array for the API call.
  $author_name = rawurlencode($author_name);
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call.
  $api_xml_result = _goodreads_request('api/author_url/' . $author_name . '.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse'];
  unset($api_result['Request']);
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * See all series by an author.
 *
 * @param string $author_id
 *   The Goodreads author ID.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_author_series($author_id) {
  // Set up the parameter array for the API call.
  $params = array(
    'id' => $author_id,
    'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call.
  $api_xml_result = _goodreads_request('series/list/', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['series_works'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * Get information on a series.
 *
 * @param string $series_id
 *   The Goodreads series ID.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_series_info($series_id) {
  // Set up the parameter array for the API call.
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call.
  $api_xml_result = _goodreads_request('series/' . $series_id . '.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['series'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * See all series a work is in.
 *
 * @param string $work_id
 *   The Goodreads work ID.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_work_series($work_id) {
  // Set up the parameter array for the API call.
  $params = array(
    'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call.
  $api_xml_result = _goodreads_request('work/' . $work_id . '/series', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['series_works'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * Get info about a group by ID.
 *
 * @param string $group_id
 *   The Goodreads group ID.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_group_by_id($group_id) {
  // Set up the parameter array for the API call.
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call.
  $api_xml_result = _goodreads_request('group/show/' . $group_id . '.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['group'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * Return members of a particular group.
 *
 * @param string $group_id
 *   The Goodreads group ID.
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the books will be downloaded.
 * @param string $sort
 *   Field to sort topics by.
 *   One of 'num_comments', 'num_books', 'date_joined', 'last_online',
 *     'first_name' (optional)
 * @param string $q
 *   List of names to search for, separating each name with a space character.
 *     Optional, will find all members by default
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_group_members($group_id, $page = '', $sort = '', $q = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up the parameter array for the API call.
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($sort != '') {
    $params['sort'] = $sort;
  }
  if ($page === '') {
    $params['page'] = 1;
    $get_all = TRUE;
  }
  else {
    $params['page'] = $page;
    $get_all = FALSE;
  }
  if ($q != '') {
    $params['q'] = str_replace(' ', '%20', $q);
  }
  $api_result_array = array();
  $api_xml_result_all = array();
  // Loop through the results..
  do {
    // Make the API call.
    $api_xml_result = _goodreads_request('group/members/' . $group_id . '.xml', $params);
    // If something went wrong in the call, return ['api_error'] = TRUE.
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    $api_result = $api_result['GoodreadsResponse']['group_users'];
    $result_count = isset($api_result['group_user']) ? count($api_result['group_user']) : 0;
    if ($result_count != 0) {
      foreach ($api_result['group_user'] as $member) {
        $api_result_array['group_user'][] = $member;
      }
      $params['page']++;
      if ($get_all && $params['page'] > $max_pages) {
        // After getting $max_pages pages, force the loop to end
        // by setting $result_count to 0.
        $result_count = 0;
      }
    }
  } while ($get_all && ($result_count != 0));

  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * List groups for a given user.
 *
 * @param string $user_id
 *   The Goodreads user ID.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_user_groups($user_id) {
  // Set up the parameter array for the API call.
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call.
  $api_xml_result = _goodreads_request('group/list/' . $user_id . '.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['groups'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * Get an user's information.
 *
 * @param string $user_id
 *   The Goodreads user ID.
 * @param string $username
 *   The Goodreads username (not the same as their name).
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_user_info($user_id = '', $username = '') {
  // Set up the parameter array for the API call.
  $params = array(
    'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($user_id != '') {
    $params['id'] = $user_id;
  }
  if ($username != '') {
    $params['username'] = urlencode($username);
  }
  // Make the API call.
  $api_xml_result = _goodreads_request('user/show/', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['user'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * View most recent user statuses on the site.
 *
 * Returns the 20 most recent user statuses posted on the Goodreads site.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_recent_user_status() {
  // Set up the parameter array for the API call.
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call.
  $api_xml_result = _goodreads_request('user_status/index.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  // ['GoodreadsResponse'] missing from this API???
  $api_result = $api_result['updates'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


// **CURRENTLY BROKEN, gets a 401 unauthorized error.
// **There is an OAuth version of the function in goodreads_oauth.inc.
/**
 * Get people who are following a user.
 *
 * @param string $user_id
 *   The Goodreads user ID.
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the books will be downloaded.
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   Help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_user_follower_list($user_id, $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up the parameter array for the API call.
  $params = array(
    // 'format' => 'xml',
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
    $api_xml_result = _goodreads_request('user/' . $user_id . '/followers.xml', $params);
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
    $result_count = 0;
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
      // After getting $max_pages pages, force the loop to end
      // by setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));

  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


// **CURRENTLY BROKEN, gets a 401 unauthorized error
// **There is an OAuth version of the function in goodreads_oauth.inc
/**
 * Get people a user is following.
 *
 * @param string $user_id
 *   The Goodreads user ID.
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the books will be downloaded.
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_user_following_list($user_id, $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up the parameter array for the API call.
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
    $api_xml_result = _goodreads_request('user/' . $user_id . '/following', $params);
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
    $result_count = 0;
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
      // After getting $max_pages pages, force the loop to end
      // by setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));

  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * Get a user's shelves.
 *
 * @param string $user_id
 *   The Goodreads user ID.
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the books will be downloaded.
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_user_shelves($user_id, $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up the parameter array for the API call.
  $params = array(
    'user_id' => $user_id,
    // 'format' => 'xml',
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
    $api_xml_result = _goodreads_request('shelf/list.xml', $params);
    // If something went wrong in the call, return ['api_error'] = TRUE.
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    $api_result = $api_result['GoodreadsResponse']['shelves'];
    $result_count = isset($api_result['user_shelf']) ? count($api_result['user_shelf']) : 0;
    if (($params['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result..
      $api_result_array = $api_result;
    }
    else {
      // Otherwise getting additional pages of books,
      // append them to the books array.
      if ($result_count != 0) {
        foreach ($api_result['user_shelf'] as $shelf) {
          $api_result_array['user_shelf'][] = $shelf;
        }
      }
    }
    $params['page']++;
    if ($get_all && $params['page'] > $max_pages) {
      // After getting $max_pages pages, force the loop to end
      // by setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));

  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * Get a list of the books on a members shelf.
 *
 * This version of the API does NOT use OAuth and viewing members
 * who have set their shelves as visible to members only or just
 * to their friends will not be returned with this function.
 * There is an alternate version using OAuth:
 *   goodreads_oauth_get_user_books($user_id....
 *
 * @param string $user_id
 *   The Goodreads user_id. (required by Goodreads)
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
 *   string containing which page to show. Options are: 1-N (optional)
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
function goodreads_get_user_books($user_id = '', $sort = '', $order = '', $shelf_name = '',
$query = '', $per_page = '', $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up parameter array for API call.
  $params = array(
     // I presume there was an earlier version of the API.
    'v' => '2',
    'key' => variable_get('goodreads_api_key', ''),
  );
  /*  This functionality doesn't work without OAuth, will generate API error
  if ($user_id === '') {
  $user_id = goodreads_oauth_get_user_id();
  $user_id = $user_id['id'];
  }
  */
  if ($sort != '') {
    $params['sort'] = $sort;
  }
  if ($order != '') {
    $params['order'] = $order;
  }
  if ($shelf_name != '') {
    $params['shelf'] = $shelf_name;
  }
  if ($query != '') {
    $params['search[query]'] = $query;
  }
  if ($per_page != '') {
    $params['per_page'] = $per_page;
  }
  if ($page === '') {
    $params['page'] = 1;
    $get_all = TRUE;
  }
  else {
    $params['page'] = $page;
    $get_all = FALSE;
  }
  if ($sort != '') {
    $params['sort'] = $sort;
  }
  $api_result_array = array();
  $api_xml_result_all = array();
  // Loop through the results.
  do {
    // Make the API call.
    $api_xml_result = _goodreads_request('review/list/' . $user_id . '.xml', $params);
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
    if (($params['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result..
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
    $params['page']++;
    if ($get_all && $params['page'] > $max_pages) {
      // After getting $max_pages pages, force the loop to end
      // by setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));

  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * Get a list of all the books on all of a members shelve.
 *
 * This version of the API does NOT use OAuth and viewing members
 * who have set their shelves as visible to members only or just
 * to their friends will not be returned with this function.
 * There is an alternate version using OAuth:
 *   goodreads_oauth_get_user_books_all_shelves($user_id....
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
function goodreads_get_user_books_all_shelves($user_id, $maximum_books_per_shelf = 100) {
  $books_per_page = floor($maximum_books_per_shelf / 5);
  $shelves = goodreads_get_user_shelves($user_id);
  if ($shelves['api_error'] === TRUE) {
    return $shelves;
  }
  $results = array();
  foreach ($shelves['user_shelf'] as $shelf) {
    $books = goodreads_get_user_books($user_id, '', '', $shelf['name'], '', $books_per_page);
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
      if (isset($books['review'])) {
        $book = $books['review']['book'];
        $results[] = _goodreads_build_book_result($book, $user_id, $shelf['name']);
      }
    }
  }
  $results['api_error'] = FALSE;
  return $results;
}


/**
 * Get list of topics in a group's folder.
 *
 * This API will get the topics from either the general folder for a group
 * given a Goodreads group ID, or it will get the topics from a specific
 * folder number.
 *
 * @param string $group_id
 *   The Goodreads group ID. (optional)
 *   If supplied and $folder_id is set or defaults to 0, then the function
 *   will return topics from the general folder for the group indicated by
 *   $group_id. If $folder_id is non-zero, this param is ignored.
 *   Note: the API may return 404 if there are no topics in the general
 *   folder for the specified group, which will return an empty result
 * @param string $folder_id
 *   Which field to search, one of title, author,
 *     genre, or all (default is all).
 * @param string $sort
 *   Field to sort topics by. One of 'title', 'updated_at',
 *     'views', 'comments_count'
 * @param string $order
 *   'a' for ascending, 'd' for descending
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the books will be downloaded.
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_topics_by_group_and_folder($group_id = '', $folder_id = '', $sort = '', $order = '', $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up the parameter array for the API call.
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($folder_id === '') {
    $folder_id = '0';
  }
  if ($group_id != '') {
    $params['group_id'] = $group_id;
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
    $api_xml_result = _goodreads_request('topic/group_folder/' . $folder_id . '.xml', $params);
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
      // After getting $max_pages pages, force the loop to end
      // by setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));
  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


/**
 * Get info about a topic by id.
 *
 * @param string $topic_id
 *   The Goodreads topic ID.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_topic_info($topic_id = '') {
  // Set up the parameter array for the API call.
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  // Make the API call.
  $api_xml_result = _goodreads_request('topic/show/' . $topic_id . '.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['topic'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 * List comments for virtually anything with comments.
 *
 * This API will get the topics from either the general folder for a group
 * given a Goodreads group ID, or it will get the topics from a specific
 * folder number.
 *
 * @param string $id
 *   The Goodreads "resource" ID. (see $type below)
 * @param string $type
 *   A string containing what kind of Goodreads "resource" to return
 *   comments for. one of 'rating', 'poll', 'photo', 'comment',
 *   'book_news_post', 'poll_vote',  'list', 'user_list_challenge',
 *   'user_challenge', 'chapter', 'blog',  'author_blog_post',
 *   'swap_request', 'quiz', 'librarian_note', 'interview',  'topic',
 *   'review', 'fanship', 'user_list_vote', 'recommendation',
 *   'owned_book', 'video', 'user_status', 'question_user_stat',
 *   'question',  'event_response', 'user', 'recommendation_request',
 *   'user_quote', 'quiz_score'
 * @param string $page
 *   Which page to show. Options are: 1-N (optional)
 *   If missing, ALL of the books will be downloaded.
 * @param string $max_pages
 *   Maximum number of pages to download.
 *   If $page is NULL, then this parameter limits the number of results to
 *   help prevent possible looping or returning many thousands of results.
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_comments($id = '', $type = '', $page = '', $max_pages = GOODREADS_MAX_PAGES) {
  // Set up the parameter array for the API call.
  $params = array(
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($type != '') {
    $params['type'] = $type;
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
    $api_xml_result = _goodreads_request('comment/index/' . $id . '.xml', $params);
    // If something went wrong in the call, return ['api_error'] = TRUE.
    if ($api_xml_result['api_error'] === TRUE) {
      return $api_xml_result;
    }
    else {
      $api_xml_result = $api_xml_result['data'];
    }
    $api_result = _goodreads_xml2array($api_xml_result);
    $api_xml_result_all[] = $api_xml_result;
    // $api_result_attr = $api_result['GoodreadsResponse']['comments_attr'];
    $api_result = $api_result['GoodreadsResponse']['comments'];
    $result_count = isset($api_result['comment']) ? count($api_result['comment']) : 0;
    if (($params['page'] == 1) || !$get_all) {
      // Only getting one page or getting first page, store whole result.
      // $api_result_array[$tag . '_attr'] = $api_result_attr;
      $api_result_array['comment'] = $api_result['comment'];
    }
    else {
      // Otherwise getting additional pages of books,
      // append them to the books array.
      if ($result_count != 0) {
        foreach ($api_result['comment'] as $comment) {
          $api_result_array['comment'][] = $comment;
        }
      }
    }
    $params['page']++;
    if ($get_all && $params['page'] > $max_pages) {
      // After getting $max_pages pages, force the loop to end
      // by setting $result_count to 0.
      $result_count = 0;
    }
  } while ($get_all && ($result_count != 0));
  $api_result_array['original_xml'] = $api_xml_result_all;
  $api_result_array['api_error'] = FALSE;
  return $api_result_array;
}


// ********************  lat and lng don't seem to work???
/**
 * Get events in your area.
 *
 * For server based inquires, you pretty much need to specify $lat and $lng.
 * Calling the API while logged in will return events associated with that
 * user.  But calling the API while logged out returns a pretty random set
 * of events, not even all in one country.
 *
 * @param string $lat
 *   The latitude to search around.
 *   (optional, but not really)
 * @param string $lng
 *   The longitude to search around.
 *   (optional, but not really)
 *
 * @return array
 *   Multi-dimensional array with result of request.
 *   return['api_error']=TRUE if something went wrong
 *      and return['data'] contains the drupal_http_request structure.
 *   return['original_xml'] contains an array of original XML responses,
 *     just in case.
 */
function goodreads_get_events($lat = '', $lng = '') {
  // Set up the parameter array for the API call.
  $params = array(
    'format' => 'xml',
    'key' => variable_get('goodreads_api_key', ''),
  );
  if ($lat != '') {
    $params['lat'] = $lat;
  }
  if ($lng != '') {
    $params['lng'] = $lng;
  }
  // Make the API call.
  $api_xml_result = _goodreads_request('event/index.xml', $params);
  // If something went wrong in the call, return ['api_error'] = TRUE.
  if ($api_xml_result['api_error'] === TRUE) {
    return $api_xml_result;
  }
  else {
    $api_xml_result = $api_xml_result['data'];
  }
  $api_result = _goodreads_xml2array($api_xml_result);
  $api_result = $api_result['GoodreadsResponse']['events'];
  $api_result['original_xml'][] = $api_xml_result;
  $api_result['api_error'] = FALSE;
  return $api_result;
}


/**
 *   *************************** SUPPORT ROUTINES ***************************
 */


/**
 * Build and get a Goodreads request.
 *
 * @param string $subpath
 *   API subpath to be called.
 * @param array $parameters
 *   Parameters for the API.
 *
 * @return array
 *   return['api_error'] = true if an error returned by API call
 *      then return['data'] contains the drupal_http_request structure.
 *   return['api_error'] = false if API call was successful
 *      and return['data'] contains the XML result of request.
 */
function _goodreads_request($subpath, $parameters = array()) {
  $request = GOODREADS_ENDPOINT . $subpath;
  $request .= (count($parameters) > 0) ? ('?' . drupal_http_build_query($parameters)) : '';
  // dpm($request); // useful when debugging API problems.
  // Is the request in our cache?
  $result = _goodreads_cache_retrieve($request);
  if ($result !== FALSE) {
    // It was in cache, return the cached result.
    $function_result['data'] = $result;
    $function_result['api_error'] = FALSE;
    return $function_result;
  }
  // Not in cache, go get it from Goodreads.
  $http_options = array(
    'headers' => array(),
    'method'  => 'GET',
    'data'    => NULL,
    'max_redirects' => GOODREADS_MAX_RETRIES,
    'timeout'       => DEFAULT_SOCKET_TIMEOUT,
  );
  $result = drupal_http_request($request, $http_options);
  $code = floor($result->code / 100) * 100;
  $function_result = array();
  if ($code == 200) {
    // Save it in our cache for later.
    _goodreads_cache_write($request, $result->data);
    // And return it.
    $function_result['data'] = $result->data;
    $function_result['api_error'] = FALSE;
    return $function_result;
  }
  else {
    // Error from Goodreads, save in dblog, return error result.
    watchdog('Goodreads', 'Goodreads API call for !path returned response code !code', array('!path' => $subpath, '!code' => $result->code), WATCHDOG_ERROR);
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
 * Look a Goodreads URL up in the cache.
 *
 * @param string $request
 *   The Goodreads request URL, used as the key.
 *
 * @return boolean|string
 *   FALSE, if the request URL was not found in the cache or if the cache
 *   entry has expired, the XML data if it was found and is not expired.
 *   If the cache entry had expired it is deleted.
 */
function _goodreads_cache_retrieve($request) {
  // Check for disabled cache.
  $cache_disabled = &drupal_static('_goodreads_disable_cache');
  if (isset($cache_disabled) && ($cache_disabled === TRUE)) {
    return FALSE;
  }
  // Cache not disabled, is the data in the cache?
  $db_result = db_select('goodreads_cache', 'cache')
    ->condition('cache_key', $request, '=')
    ->fields('cache', array('cache_data', 'cache_time'))
    ->execute()
    ->fetchAll();
  if (empty($db_result[0])) {
    return FALSE;
  }
  if ((REQUEST_TIME - $db_result[0]->cache_time) > variable_get('goodreads_cache_lifetime', 0)) {
    // Cache has expired (longer than time set in configuration),
    // delete old value and return FALSE.
    db_delete('goodreads_cache')
      ->condition('cache_key', $request)
      ->execute();
    return FALSE;
  }
  // In cache and still valid, return the result value
  // dpm('cache hit'); // if debugging.
  return $db_result[0]->cache_data;
}


/**
 * Write a Goodreads URL and its data into the cache.
 *
 * @param string $request
 *   The Goodreads request URL, used as the key.
 * @param string $data
 *   The Goodreads data to save.
 * @param integer $uid
 *   The Drupal User ID, default to 0.
 *
 * @return array
 *   nothing. function is a "roach motel" data goes in but it doesn't come out.
 */
function _goodreads_cache_write($request, $data, $uid = 0) {
  // Check for disabled cache.
  $cache_disabled = &drupal_static('_goodreads_disable_cache');
  if (isset($cache_disabled) && ($cache_disabled === TRUE)) {
    return;
  }
  // dpm('cache write'); // If debugging.
  // Cache not disabled, write the data to the cache.
  $sql = array(
    'cache_uid' => $uid,
    'cache_time' => REQUEST_TIME,
    'cache_key' => $request,
    'cache_data' => $data,
  );
  drupal_write_record('goodreads_cache', $sql);
  return;
}


/**
 * Temporarily disable the Goodreads cache.
 *
 * Calling this function will disable the Goodreads cache for this page
 * request, allowing more recent data to be retrieved from Goodreads.
 * Note that this function does not clear the cached data so another
 * call on another page could end up with cached data.
 */
function goodreads_cache_disable() {
  $cache_disabled = &drupal_static('_goodreads_disable_cache');
  $cache_disabled = TRUE;
}


/**
 * Re-enable the Goodreads cache.
 *
 * Calling this function will disable the Goodreads cache for this page
 * request, allowing more recent data to be retrieved from Goodreads.
 * Note that this function does not clear the cached data so another
 * call on another page could end up with cached data.
 */
function goodreads_cache_enable() {
  $cache_disabled = &drupal_static('_goodreads_disable_cache');
  if (isset($cache_disabled)) {
    drupal_static('_goodreads_disable_cache', NULL, TRUE);
  }
}


/**
 * Convert Goodreads XML to an object.
 *
 * @param string $xml
 *   The Goodreads XML object.
 *
 * @return object
 *   Equivalent object.
 */
function _goodreads_xml2object($xml) {
  return simplexml_load_string($xml, 'SimpleXMLElement', LIBXML_NOCDATA);
}


/**
 * Convert Goodreads XML to an array.
 *
 * Based on xml2array() by Binny V A :
 *   http://www.bin-co.com/php/scripts/xml2array/
 * And I changed how attributes are returned within the array.
 *
 * @param string $contents
 *   The Goodreads XML object.
 * @param integer $get_attributes
 *   1 or 0. If this is 1 the function will get the attributes as well as
 *     the tag values - this results in a different array structure in the
 *     return value.
 * @param string $priority
 *   Can be 'tag' or 'attribute'. This will change the way the resulting
 *      array structure.  For 'tag', the tags are given more importance.
 *
 * @return array
 *   Equivalent array.
 */
function _goodreads_xml2array($contents, $get_attributes = 1, $priority = 'tag') {
  // Check for empty input, return empty array.
  if (!$contents) {
    return array();
  }

  // Check for required library, alert user and db_log if not there,
  // return empty array,
  // should never be seen in production, most likely seen when you
  // enable the module and try it.
  if (!function_exists('xml_parser_create')) {
    // Get the XML parser of PHP - PHP must have this module
    // for the parser to work.
    drupal_set_message(t('Unable to find PHP parser. This module needs php-xml lib.'), 'error');
    watchdog('Goodreads', 'Unable to find PHP parser. This module needs php-xml lib.', array(), WATCHDOG_ERROR);
    return array();
  }

  // Get the XML parser of PHP -
  // PHP must have this module for the parser to work.
  // Requirement checked in goodreads_requirements.
  $parser = xml_parser_create('');
  xml_parser_set_option($parser, XML_OPTION_TARGET_ENCODING, "UTF-8");
  // http://minutillo.com/steve/weblog/2004/6/17/
  // php-xml-and-character-encodings-a-tale-of-sadness-rage-and-data-loss.
  xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
  xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, 1);
  xml_parse_into_struct($parser, trim($contents), $xml_values);
  xml_parser_free($parser);

  if (!$xml_values) {
    // Hmm...
    return;
  }

  // Initializations.
  $xml_array = array();
  $parents = array();
  $opened_tags = array();
  $arr = array();
  // Reference.
  $current = &$xml_array;

  // Go through the tags.
  // Multiple tags with same name will be turned into an array.
  $repeated_tag_index = array();
  foreach ($xml_values as $data) {
    // Remove existing values, or there will be trouble.
    unset($attributes, $value);
    // This command will extract these variables into the foreach scope.
    // tag(string), type(string), level(int), attributes(array).
    // We could use the array by itself, but this cooler.
    extract($data);
    $result = array();
    $attributes_data = array();
    if (isset($value)) {
      if ($priority == 'tag') {
        $result = $value;
      }
      // Put the value in a assoc array if we are in the 'Attribute' mode.
      else {
        $result['value'] = $value;
      }
    }
    // Set the attributes too.
    if (isset($attributes) and $get_attributes) {
      foreach ($attributes as $attr => $val) {
        if ($priority == 'tag') {
          $attributes_data[$attr] = $val;
        }
        // Set all the attributes in a array called 'attr'.
        else {
          $result['attr'][$attr] = $val;
        }
      }
    }
    // See tag status and do the needed.
    // The starting of the tag '<tag>'.
    if ($type == "open") {
      $parent[$level - 1] = &$current;
      if (!is_array($current) or (!in_array($tag, array_keys($current)))) {
        // Insert New tag.
        $current[$tag] = $result;
        if ($attributes_data) {
          $current[$tag][$tag . '_attr'] = $attributes_data;
        }
        $repeated_tag_index[$tag . '_' . $level] = 1;
        $current = &$current[$tag];
      }
      else {
        // There was another element with the same tag name.
        // If there is a 0th element it is already an array.
        if (isset($current[$tag][0])) {
          $current[$tag][$repeated_tag_index[$tag . '_' . $level]] = $result;
          $repeated_tag_index[$tag . '_' . $level]++;
        }
        else {
          // This section will make the value an array if multiple tags
          // with the same name appear together.
          // This will combine the existing item and the new item together
          // to make an array.
          $current[$tag] = array($current[$tag], $result);
          $repeated_tag_index[$tag . '_' . $level] = 2;
          // The attribute of the last(0th) tag must be moved as well.
          if (isset($current[$tag][$tag . '_attr'])) {
            $current[$tag]['0_attr'] = $current[$tag][$tag . '_attr'];
            unset($current[$tag][$tag . '_attr']);
          }
        }
        $last_item_index = $repeated_tag_index[$tag . '_' . $level] - 1;
        $current = &$current[$tag][$last_item_index];
      }
    }
    // Tags that ends in 1 line '<tag />'.
    elseif ($type == "complete") {
      // See if the key is already taken.
      // New Key.
      if (!isset($current[$tag])) {
        if ($priority == 'tag' and $attributes_data) {
          $current[$tag]['value'] = $result;
          $current[$tag][$tag . '_attr'] = $attributes_data;
        }
        else {
          $current[$tag] = $result;
        }
        $repeated_tag_index[$tag . '_' . $level] = 1;
      }
      else {
        // If taken, put all things inside a list(array).
        if (isset($current[$tag][0]) and is_array($current[$tag])) {
          // If it is already an array...
          // ...push the new element into that array.
          $current[$tag][$repeated_tag_index[$tag . '_' . $level]] = $result;
          if ($priority == 'tag' and $get_attributes and $attributes_data) {
            $current[$tag][$repeated_tag_index[$tag . '_' . $level]][$tag . '_attr'] = $attributes_data;
          }
          $repeated_tag_index[$tag . '_' . $level]++;
        }
        else {
          // If it is not an array...
          // Make it an array using using the existing value and the new value.
          $current[$tag] = array($current[$tag], $result);
          $repeated_tag_index[$tag . '_' . $level] = 1;
          if ($priority == 'tag' and $get_attributes) {
            // The attribute of the last(0th) tag must be moved as well.
            if (isset($current[$tag . '_attr'])) {
              $current[$tag]['0_attr'] = $current[$tag][$tag . '_attr'];
              unset($current[$tag][$tag . '_attr']);
            }
            if ($attributes_data) {
              $current[$tag][$repeated_tag_index[$tag . '_' . $level]][$tag . '_attr'] = $attributes_data;
            }
          }
          // 0 and 1 index is already taken.
          $repeated_tag_index[$tag . '_' . $level]++;
        }
      }
    }
    elseif ($type == 'close') {
      // End of tag '</tag>'.
      $current = &$parent[$level - 1];
    }
  }
  return ($xml_array);
}


/**
 * Access an array using keys stored in a variable.
 *
 * Got it via Google from
 * http://stackoverflow.com/questions/4842927/
 *     php-variable-variable-array-key-list
 *
 * @param array $array
 *   An array, single or multidimensional, e.g. $book.
 * @param string $keys
 *   The keys for the item to access, include the square
 *   brackets, but don't use quotes, e.g. '[book][title]'.
 *
 * @return array|string
 *   Resulting value (could be an array), e.g. $book['book']['title'].
 */
function _goodreads_variable_array(array $array, $keys) {
  if (!is_array($array)) {
    // Not an array nothing to do.
    return NULL;
  }
  if (!preg_match_all('~\[([^\]]+)\]~', $keys, $matches, PREG_PATTERN_ORDER)) {
    // If keys aren't in the form of [..] nothing to do.
    return NULL;
  }
  $keys = $matches[1];
  $current = $array;
  foreach ($keys as $key) {
    $current = $current[$key];
  }
  return $current;
}


/**
 * Display an array's structure.
 *
 * @param array $array
 *   An array, single or multidimensional, e.g. $book.
 * @param boolean $display_values
 *   TRUE if array values should be displayed, default FALSE
 * @param string $separator
 *   A string containing a separator for the array element listings.
 *   Most likely '<br />' for web page or "\r\n" for file
 * @param string $lbracket
 *   A string for the left side of a tag, defaults to '['
 * @param string $rbracket
 *   A string for the right side of a tag, defaults to ']'
 * @param boolean $only_zero_index
 *   TRUE if only zero numerical indexes, default FALSE
 *     other numerical indexes converted to '*'
 * @param integer $level
 *   Internal parameter used to control infinite recursion, set to 10
 * @param string $parent
 *   Internal parameter used to track higher level
 * Last two parameters are NOT passed when calling the function.
 *
 * @return string
 *   String showing array's structure.
 */
function _goodreads_array_structure($array, $display_values = FALSE, $separator = '<br />', $lbracket = '[', $rbracket = ']', $only_zero_index = FALSE, $level = 0, $parent = NULL) {
  if ($level > 10) {
    $parent .= 'too many recursion levels';
    return $parent;
  }
  $result = '';
  foreach ($array as $key => $value) {
    // If the value is an array, call the function recursively.
    if (is_array($value)) {
      $key = ($only_zero_index && is_numeric($key) && ($key != 0)) ? '*' : $key;
      $result .= _goodreads_array_structure($value, $display_values, $separator, $lbracket, $rbracket, $only_zero_index, ($level + 1), ($parent . $lbracket . $key . $rbracket));
    }
    // Not an array, time to add the result to the function result.
    else {
      $key = (is_numeric($key) && ($key != 0)) ? '*' : $key;
      $result .= $parent . $lbracket . $key . $rbracket . ($display_values ? (': ' . $value) : NULL) . $separator;
    }
  }
  return $result;
}


/**
 * Display array, elimiate duplicates.
 *
 * Display an array's structure eliminating duplicate array elements referenced
 * by numeric indexes.
 *
 * @param array $array
 *   An array, single or multidimensional, e.g. $book.
 * @param boolean $display_values
 *   TRUE if array values should be displayed, default FALSE
 * @param string $separator
 *   A string containing a separator for the array element listings.
 *   Most likely '<br />' for web page or "\r\n" for file
 * @param string $lbracket
 *   A string for the left side of a tag, defaults to '['
 * @param string $rbracket
 *   A string for the right side of a tag, defaults to ']'
 *
 * @return string
 *   String showing array's structure.
 */
function _goodreads_array_structure_nodups($array, $display_values = FALSE, $separator = '<br />', $lbracket = '[', $rbracket = ']') {
  $result = _goodreads_array_structure($array, $display_values, $separator, $lbracket, $rbracket, TRUE);
  $array = explode($separator, $result);
  foreach ($array as $key => $value) {
    if (!(strpos($value, '*') === FALSE)) {
      unset($array[$key]);
    }
  }
  // Because php.net says so.
  unset($value);
  return implode($separator, $array);
}


/**
 * Test to see if we are using Goodreads OAuth.
 *
 * @return boolean
 *   TRUE if OAuth module enabled AND we have the consumer key and secret.
 */
function _goodreads_use_oauth() {
  if (!module_exists('oauth_common')) {
    return FALSE;
  }
  return _goodreads_oauth_ready();
}


/**
 * Test to see if we are ok to use Goodreads OAuth.
 *
 * @return boolean
 *   TRUE if we have the consumer key and secret.
 */
function _goodreads_oauth_ready() {
  $result = (variable_get('goodreads_api_key', '') && variable_get('goodreads_api_secret', ''));
  if (!$result) {
    drupal_set_message(t('The Goodreads API keys have not been set.'), 'error');
    // I would have used this message but never figured out a way
    // to get the link in without upsetting Coder
    // drupal_set_message(t('The Goodreads API keys have not been set, !link',
    // array('!link' => l(t('enter them now'),
    // 'admin/config/content/goodreads'))),
    // 'error');
  }
  return $result;
}


/**
 * Clean and validate an ISBN.
 *
 * @param string $isbn
 *   The ISBN.
 *
 * @return boolean|string
 *   clean ISBN or FALSE.
 */
function _goodreads_validate_isbn($isbn) {
  // Remove ISBN tag if present.
  $result = str_replace('isbn', '', drupal_strtolower($isbn));
  // Remove any spaces.
  $result = str_replace(' ', '', $result);
  // Remove any dashes.
  $result = str_replace('-', '', $result);
  if (_goodreads_is_valid_isbn($result)) {
    return $result;
  }
  else {
    return FALSE;
  }
}

/**
 * Test whether an ISBN number is valid.
 *
 * @param string $number
 *   The ISBN.
 *
 * @return boolean
 *   TRUE or FALSE.
 */
function _goodreads_is_valid_isbn($number) {
  if (drupal_strlen($number) == 10) {
    // Brute force calculation.
    $checkdigit = 11 - ((10 * drupal_substr($number, 0, 1) + 9 * drupal_substr($number, 1, 1) + 8 * drupal_substr($number, 2, 1) + 7 * drupal_substr($number, 3, 1) + 6 * drupal_substr($number, 4, 1) + 5 * drupal_substr($number, 5, 1) + 4 * drupal_substr($number, 6, 1) + 3 * drupal_substr($number, 7, 1) + 2 * drupal_substr($number, 8, 1)) % 11);
    switch ($checkdigit) {
      case 10:
        $checkdigit = 'X';
        break;

      case 11:
        $checkdigit = 0;
        break;

      default:
    }
    if ($checkdigit == drupal_substr($number, 9, 1)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  elseif (drupal_strlen($number) == 13) {
    // Brute force calculation.
    $checkdigit = 10 - ((1 * drupal_substr($number, 0, 1) + 3 * drupal_substr($number, 1, 1) + 1 * drupal_substr($number, 2, 1) + 3 * drupal_substr($number, 3, 1) + 1 * drupal_substr($number, 4, 1) + 3 * drupal_substr($number, 5, 1) + 1 * drupal_substr($number, 6, 1) + 3 * drupal_substr($number, 7, 1) + 1 * drupal_substr($number, 8, 1) + 3 * drupal_substr($number, 9, 1) + 1 * drupal_substr($number, 10, 1) + 3 * drupal_substr($number, 11, 1)) % 10);
    if ($checkdigit == 10) {
      $checkdigit = '0';
    }
    if ($checkdigit == drupal_substr($number, 12, 1)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}
/**
 * Get the relevant information from a Goodreads book.
 *
 * Simple helper function to format the data for *get_users_books
 *
 * @param array $book
 *   Goodreads values for a book.
 * @param string $user_id
 *   oodreads user ID that this book is for.
 * @param string $shelf_name
 *   Goodreads shelf name this book is from.
 *
 * @return array
 *   Array with selected data from the book
 */
function _goodreads_build_book_result($book, $user_id, $shelf_name) {
  if (!is_array($book)) {
    // Houston we have a problem
    // dpm($book, 'build_book_result');
    return '';
  }
  $results = array();
  $results['goodreads_user_id'] = _goodreads_force_string($user_id);
  $results['shelf_name'] = _goodreads_force_string($shelf_name);
  $results['title'] = _goodreads_force_string($book['title']);
  $results['author'] = _goodreads_force_string(_goodreads_extract_author_name($book));
  $results['ratings_count'] = _goodreads_force_string($book['ratings_count']);
  $results['average_rating'] = _goodreads_force_string($book['average_rating']);
  $results['text_reviews_count'] = _goodreads_force_string($book['text_reviews_count']['value']);
  $results['image_url'] = _goodreads_force_string($book['image_url']);
  $results['isbn'] = _goodreads_force_string($book['isbn']);
  $results['goodreads_book_id'] = _goodreads_force_string($book['id']['value']);
  $results['description'] = _goodreads_force_string($book['description']);
  return $results;
}


/**
 * Force a something into a string.
 *
 * @param string $parm
 *   In theory a string.
 *
 * @return string
 *   If a string is passed return it, otherwise an empty string.
 */
function _goodreads_force_string($parm) {
  return (is_string($parm) ? $parm : '');
}


/**
 * Get the author from a Goodreads book.
 *
 * Since the XML2ARRAY for Goodreads books can return different structures
 * if there is a single author or multiple authors, this function returns
 * the primary author either way.
 *
 * @param array $book
 *   An array containing the Goodreads values for a book.
 *
 * @return string
 *   String with the primary author's name
 */
function _goodreads_extract_author_name($book) {
  if (!is_array($book)) {
    // Houston we have a problem
    // dpm($book, 'extract_author_name');
    return '';
  }
  if (isset($book['authors']['author'][0])) {
    // var_dump('isset extract author', $book['authors']['author'][0]['name']);
    return $book['authors']['author'][0]['name'];
  }
  else {
    // var_dump('NOT isset extract author', $book['authors']['author']['name']);
    return $book['authors']['author']['name'];
  }
}
