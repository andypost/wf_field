<?php 
/**
 * Decorator class for entities to improve/ease developer experience
 * 
 * @author frega
 */
class WfFieldEntityWrapper {
  /**
   * Entity object this object is wrapped around 
   * @var stdClass
   */
  private $entity;
  
  /**
   * Entity object this object is wrapped around 
   * @var stdClass
   */
  private $entity_type;
  
  /**
   * Entity object this object is wrapped around 
   * @var stdClass
   */
  private $bundle_name;
  
  /**
   * Entity.module wrapped entity 
   * @var stdClass
   */
  private $wrapped_entity;
  
  /**
   * Active workflow field name 
   * @var string
   */
  private $wf_field_field_name;
  
  /**
   * Default workflow field name 
   * @var string
   */
  private $wf_field_default_field_name;
  
  /**
   * Options passed 
   * @var  array
   */
  private $options=array();
  
  /**
   * Sets the entity this object wraps 
   * @param 	 object $entity
   * @return   wf_field_entity_wrapper
   */
  private function _setEntity( $entity, $options=array() ) {
    $this->entity = $entity;
    if ( $this->entity_type == 'node' ) {
      $this->bundle_name = $entity->type;
    }
    return $this;
  }

  /**
   * Retrieve the wrapped Entity
   */
  private function getWrappedEntity() {
    if (!isset($this->_wrapped_entity)) {
      $this->wrapped_entity = entity_metadata_wrapper($this->entity_type, $this->entity);
    } 
    return $this->wrapped_entity;
  }
  
  /**
   * Constructor
   * @param		object	$entity
   */
  function __construct( $entity_type, $entity, $options=array() ) {
    $this->entity_type = 'node';
    $this->_setEntity( $entity, $options );        
    $this->options = $options;
  }
  
  /**
   * Does this entity instance have wf_field fields attached
   * @return	bool	true if it has at least one
   */
  function hasFields() {
    $names = $this->getWfFieldNames();
    return (bool) sizeof($names);
  }
  
  /**
   * Return the field instances for all wf_fields on this entity instance
   * @param string/NULL 	optionally get a single field instance
   * @param array 				either array of field instance-arrays or field instance array for $field_name-field 
   */
  function getFieldInstances($field_name=NULL) {
    if (!isset($this->_field_instances)) {
      // @todo fix for other entities
      $this->_field_instances = _wf_field_get_all_instances($this->entity_type, $this->bundle_name);
    }
    if (isset($field_name)) {
      // @todo improve this - the array should be pre-keyed accordingly
      foreach ($this->_field_instances as $instance) {
         if ($instance['field_name'] == $field_name) {
           return $instance;
         }
      } 
      return NULL;
    }
    else {
      return $this->_field_instances;
    }
  }
  
  /**
   * Return a field instance array by field name 
   * @param string  			field name
   * @param array 				field instance array for $field_name-field 
   */
  function getFieldInstance( $field_name=NULL ) {
    if (!isset($field_name)) {
      $field_name = $this->_getActiveFieldName();
    }
    return $this->getFieldInstances( $field_name );
  }
  
  /**
   * Return field names of wf_field attached to this entity
   * @return array	
   */
  function getFieldNames() {
    $names = array();    
    foreach ( (array) $this->getFieldInstances() as $k=>$v) {
      $names[] = $v['field_name'];
    }
    return $names;
  }
  
  function getCurrentTransitionOptions($options) {
    if (!isset($options['account'])) {
      $options['account'] = $GLOBALS['user'];
    }
    return wf_field_get_current_transition_options($this->entity, $this->_getActiveFieldName(), $options);
  }
  
  /**
   * Returns the target state 
   * @param string 	a
   */
  function _resolveTransitionTargetStateId($transition_state_name_label, $options=array() ) {
    // 
  }
  
	/**
   * Returns 
   * @param string 	a
   */
  function _resolveTransitionId($transition_state_name_label, $options=array() ) {
    // 
  }
  
  /**
   * Return available transitions 
   * @param array $options
   */
  function getAvailableTransitions($options=array() ) {
    return $this->getCurrentTransitionOptions($options);    
  }
  
	/**
   * Return available transitions 
   * @param array $options
   */
  function getAvailableTargetStates($options=array()) {
    // make sure we get the state ids back
    $options['index_by_state_id'] = true;
    return wf_field_get_current_transition_options($options);
  }
  
  function getAvailableTransitionsInfo($options=array()) {
    // make sure we get complete infos back
    $options['complete_info'] = true;
    return $this->getAvailableTransitions($options);
  }
  
  /**
	 * Return the value of the current
   * @param unknown_type $options
   */
  function value($options=array()) {
    $fn = $this->_getActiveFieldName();
    // $value = $this->getWrappedEntity()->{$fn}->value();
    $language = isset($this->entity->language) ? $this->entity->language : 'und';
    $value = isset($this->entity->{$fn}[ $language ][0]['wf_state']) ?  $this->entity->{$fn}[ $language ][0]['wf_state'] : NULL;
    return $value;
  }
  
  
  /**
   * Trigger a transition 
   * Enter description here ...
   * @param unknown_type $target_state_or_options
   */
  function transition($target_state, $options=array()) {
    return wf_field_transition_execute($this->entity, $this->_getActiveFieldName(), $target_state, $options);
  }
  
  // {{{ DEFAULT FIELD AND SWITCHING FIELDS (PREPARATION FOR MULTIPLE FIELDS PER ENTITY) 
  /**
   * Returns the active wf_field field name that we are operating on 
   * @return  string  field name
   */
  private function _getActiveFieldName() {
    if (!isset($this->wf_field_field_name)) {
      if ( isset($this->wf_field_default_field_name) ) {
        $this->wf_field_field_name = $this->wf_field_default_field_name;
      } 
      else { 
        // get the first wf_field-field_name
        $names = $this->getFieldNames();
        $this->wf_field_field_name = $names[0];
      }
    } 
    return $this->wf_field_field_name;
  }
  
  /**
   * Allow specifying to which field the actions apply
   * Because entities can have multiple wf_field-fields this allows to select one
   * @param		string		field name
   * @return	object	wf_field_wrapper
   */
  function withField($field_name) {
    $names = $this->getFieldNames();
    if (in_array($field_name, $names)) {
      $this->wf_field_field_name = $field_name;
    } else {
      throw new Exception('Invalid field name');
    }
    return $this;
  }

  /**
   * Uses the default (first) field 
   * @return	object	wf_field_wrapper
   */
  function withDefaultField() {
    unset( $this->wf_field_field_name );
    $this->_getWfFieldName();
    return $this;
  }
  
  /**
   * Set a default field
   * @return	object	wf_field_wrapper
   */
  function setDefaultField($field_name) {
    $names = $this->getFieldNames();
    if (in_array($field_name, $names)) {
      $this->wf_field_default_field_name = $field_name;
    } else {
      throw new Exception('Invalid field name');
    }
    return $this; 
  }

}