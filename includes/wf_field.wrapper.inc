<?php 
/**
 * Decorator class for entities to improve/ease developer experience
 * 
 * @author frega
 */
class WfFieldWrapper {
  /**
   * Entity object this object is wrapped around 
   * @var stdClass
   */
  private $entity;
  
  /**
   * Active workflow field name 
   * @var string
   */
  private $wf_field_field_name;
  
  /**
   * Options passed 
   * @var  array
   */
  private $options=array();
  
  /**
   * Sets the entity this object wraps 
   * @param 	 object $entity
   * @return   wf_field_wrapper
   */
  private function _setEntity( $entity, $options=array() ) {
    $this->entity = $entity;
    $entity->_wf_field_wrapper = $this;
    return $this;
  }
  
  /**
   * Constructor
   * @param		object	$entity
   */
  function __construct( $entity, $options=array() ) {
    $this->_setEntity( $entity, $options );        
    $this->options = $options;
  }
  
  function hasWfFields() {
    
  }
  
  function getWfFields() {
    
  }
  
  function getAvailableTransitions($options=NULL) {
   
  }
  
  function triggerTransition($target_state_or_options=NULL) {
    
  }
  
  private function _getWfFieldName() {
    if (!isset($this->wf_field_field_name)) {
      // get the first wf_field-field_name
    }
  }
  
  /**
   * Allow specifying to which field the actions apply
   * Because entities can have multiple wf_field-fields this allows to select one
   * @param		string		field name
   * @return	object	wf_field_wrapper
   */
  function withField($field_name) {
    $this->wf_field_field_name =  $field_name;
    return $this;
  }

  /**
   * Uses the default (first) field 
   */
  function withDefaultField() {
    unset( $this->wf_field_field_name) ;
    return $this->getWfFields();
  }
  
  function setDefaultField($field_name) {
    $this->wf_field_default_field_name = $field_name; 
  }

}