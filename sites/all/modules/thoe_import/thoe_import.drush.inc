<?php 

/**
 * Implements hook_drush_command().
 */
function thoe_import_drush_command(){
  $items['thoe_import'] = array(
      'description' => 'Import content from different sources',
      'arguments' => array(
          'service' => '',
          'topic' => ''
      ),
      'aliases' => array('timp'),
      'examples' => array(
          'drush timp service' => 'Import data from a service',
      ),
  );
  $items['thoe_import_list'] = array(
      'description' => 'Import content from different sources',
      'aliases' => array('timpl'),
      'examples' => array(
          'drush timpl' => 'List available services',
      ),
  );
  $items['thoe_import_fix_text'] = array(
      'description' => 'Fix text in nodes',
      'aliases' => array('thoeft'),
      'examples' => array(
          'drush thoeft' => 'Fix text in nodes',
      ),
  );
  
  $items['thoe_import_remove_duplicates'] = array(
      'description' => 'Remove duplicates',
      'aliases' => array('thoerd'),
      'examples' => array(
          'drush thoerd' => 'Remove duplicates',
      ),
  );
  
  $items['thoe_import_remove_videos'] = array(
      'description' => 'Remove trash videos',
      'aliases' => array('thoerv'),
      'examples' => array(
          'drush thoerv' => 'Remove trash videos',
      ),
  );
  
  $items['thoe_import_translate'] = array(
      'description' => 'Translate the nodes into different languages',
      'arguments' => array(
          'language' => '',
      ),
      'aliases' => array('thoe_translate'),
      'examples' => array(
          'drush thoe_translate tr' => 'Translate nodes',
      ),
  );
  
  return $items;
}

function drush_thoe_import_list() {
  $enabled_modules = thoe_import_enabled_modules();
  $list = implode("\n",$enabled_modules);
  $list = str_replace("thoe_import_","",$list);
  return $list;
}

function drush_thoe_import_fix_text() {
  thoe_import_fix_text();
}

function thoe_import_enabled_modules() {
  $enabled_modules = db_query("SELECT name FROM {system} WHERE type = 'module' AND status = 1 AND name LIKE :module_name",array(':module_name'=> db_like("thoe_import_") . "%"))->fetchAll(PDO::FETCH_ASSOC);
  $modules = array();
  foreach ($enabled_modules as $module) {
  	$modules[] = $module["name"];
  }
  
  return $modules;
}

function drush_thoe_import($service,$topic){
  
  $enabled_modules = thoe_import_enabled_modules();
  
  if(in_array("thoe_import_".$service, $enabled_modules)) {
    drush_print("Service ".$service." enabled, will process");
    call_user_func("thoe_import_".$service."_start",$topic);
  }
  else
  {
    drush_print("Service disabled, exit");
  }
}

function drush_thoe_import_translate($language){
  thoe_import_translate_mass($language);
}

function drush_thoe_import_remove_videos() {
  thoe_import_youtube_clean();
}

function drush_thoe_import_remove_duplicates() {
  thoe_import_delete_duplicates();
}










