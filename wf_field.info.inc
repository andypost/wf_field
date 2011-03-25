<?php 
/**
 * Additional callback to adapt the property info for term reference fields.
 * @see entity_metadata_field_entity_property_info().
 */
function wf_field_property_info_callback(&$info, $entity_type, $field, $instance, $field_type) {
  $name = $field['field_name'];
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$name];
  $property['options list'] = 'wf_field_property_options_list';
  // getter callback returns an instance of WfFieldPropertyWrapper 
  $property['getter callback'] = 'wf_field_property_get';
  // @todo: test this
  $property['setter callback'] = 'wf_field_property_set';
  $property['raw getter callback'] = 'wf_field_property_get_raw';
  // expose the properties available in the  
  $property['property info'] = array(
    'wf_state' => array( 
      'label' => t('Workflow State Id'),
      'description' => !empty($name) ? t('Workflow state id of field %name', array('%name' => $name)) : '',
      'type' => 'integer',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'wf_state_old' => array( 
      'label' => t('Workflow State Id (initial / before saving)'),
      'description' => !empty($name) ? t('Workflow state id of field %name', array('%name' => $name)) : '',
      'type' => 'integer',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),    
  );
  
  unset($property['query callback']);
}



/**
 * Callback returning the options list of a field.
 */
function wf_field_property_options_list($name, $info) {
  return entity_metadata_field_options_list($name, $info);
}