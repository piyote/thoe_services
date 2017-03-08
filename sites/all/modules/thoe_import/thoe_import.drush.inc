<?php 

/**
 * Implements hook_drush_command().
 */
function thoe_import_drush_command(){
  $items['thoe_import'] = array(
      'description' => 'Import content from different sources',
      'arguments' => array(
          'service' => '',
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
  return $items;
}

function drush_thoe_import_list() {
  $enabled_modules = thoe_import_enabled_modules();
  $list = implode("\n",$enabled_modules);
  $list = str_replace("thoe_import_","",$list);
  return $list;
}

function thoe_import_enabled_modules() {
  $enabled_modules = db_query("SELECT name FROM {system} WHERE type = 'module' AND status = 1 AND name LIKE :module_name",array(':module_name'=> db_like("thoe_import_") . "%"))->fetchAll(PDO::FETCH_ASSOC);
  $modules = array();
  foreach ($enabled_modules as $module) {
  	$modules[] = $module["name"];
  }
  
  return $modules;
}

function drush_thoe_import($service){
  
  $enabled_modules = thoe_import_enabled_modules();
  
  if(in_array("thoe_import_".$service, $enabled_modules)) {
    drush_print("Service enabled, will process");
    call_user_func("thoe_import_".$service."_start");
  }
  else
  {
    drush_print("Service disabled, exit");
  }
}













