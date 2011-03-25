<?php 
/**
 * Wrapper class for wf_field-property instances
 * 
 * @author frega
 */
class WfFieldPropertyWrapper {
  
  function __construct($entity, array $options, $name, $entity_type, &$context) {
    $this->entity = $entity;
    $this->entity_type = $entity_type;
    $this->options = $options;
    $this->name = $name;
    $this->context = $context;
  }
  
  /**
   * Returns the 
   * Enter description here ...
   */
  function getFieldInstance() {
    if (!isset($this->context['instance'])) {
      $this->context['instance'] = field_info_instance($this->context['parent']->type(), $this->name, $this->context['parent']->getBundle());
    }
    return $this->context['instance'];
  }
  

  /** 
   * Returns the (raw) scalar value 
   */
  function getRawValue() {
    // Set contextual info useful for getters of any child properties.
    $this->context['instance'] = field_info_instance($this->context['parent']->type(), $this->name, $this->context['parent']->getBundle());
    $this->context['field'] = field_info_field($this->name);
  
    $langcode = field_language($this->entity_type, $this->entity, $this->name, isset($this->options['language']) ? $this->options['language']->language : NULL);
    $name  = $this->name; 
    return isset($this->entity->{$name}[$langcode][0]['wf_state']) ? $this->entity->{$name}[$langcode][0]['wf_state'] : NULL;
  }
  
  /**
   * 
   * Enter description here ...
   */
  function getStateLabel() {
    return wf_field_get_current_state_label($this->getRawValue(), $this->getFieldInstance() );
  }
  
  /**
   * Magic toString conversion
   * @return int/null 
   */
  function __toString() {
    return $this->getRawValue();
  }
  
}
