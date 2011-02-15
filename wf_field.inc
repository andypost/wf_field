<?php
// $Id$

/**
 * @file
 *
 * API / Internal helper functions
 */

/**
 * Helper function that returns the appropriate field info for a range of inputs
 * - instance-array
 * - field_name 
 * - field_id
 * - field-array
 *  
 * Enter description here ...
 * @param array/string  either a field
 */
function _wf_field_get_field_info($array_or_string) {
  if (is_string($array_or_string)) {
    if (is_numeric($array_or_string)) {
      $field = field_info_field_by_id($array_or_string);
    } else {
      $field = field_info_field($array_or_string);
    }
  }
  elseif (isset($array_or_string['field_name'])) {
    $field = field_info_field($array_or_string['field_name']);
  } 
  else {
    $field = field_info_field_by_id($array_or_string['field_id']);
  }
  return $field;
}

/**
 * Return all states
 * @param	array		field instance array
 * @param unknown_type $options  passive => TRUE - return (optional) passive label (i.e. Published instead of Publication)
 */
function _wf_field_get_states($instance_or_fieldname, $options=array()) {
  $field = _wf_field_get_field_info($instance_or_fieldname);  
  if (!isset($field['settings']['wf_settings']['wf_states']['text']  )) {
    return FALSE;
  }
  $raw_states = explode("\n", trim($field['settings']['wf_settings']['wf_states']['text']  ));

  // do we want to return the "passive" expression (Published) instead of (Publication)
  $passive = isset($options['passive']) && $options['passive'];

  // Build an array of workflow states on the form (number => name of state).
  // If no state name is given, use the state integer instead.
  $states = array();
  foreach ($raw_states as $state) {
    $parts = explode('|', $state);
    if (strlen($parts[0])) {
      if ($passive && isset($parts[2])) {
        $states[$parts[0]] = trim( ($parts[2]) ? $parts[2] : trim( ($parts[1]) ? $parts[1] : $parts[0] ) );
      }
      else {
        $states[$parts[0]] = trim( ($parts[1]) ? $parts[1] : $parts[0] );
      }
    }
  }
  return $states;
}

/**
 * Validate the workflow states definition string in the field instance settings.
 * @param string $string
 * @param array $options
 * @return bool	  true if validate states string else false
 */
function _wf_field_validate_states_string($string, $options=array()) {
  $raw_states = explode("\n", trim($string));
  // Build an array of workflow states on the form (number => name of state).
  // If no state name is given, use the state integer instead.
  $states = array();
  foreach ($raw_states as $state) {
    $parts = explode('|', $state);
    // if there isnt a state_id, or a duplicate-state id or a non integer 
    if (!strlen($parts[0]) || isset($states[$parts[0]]) || ( (int) $parts[0] != $parts[0] )) {
      return FALSE;
    } 
    else {
      $states[$parts[0]] = $parts[1]; 
    }
  }
  return TRUE;
}

/**
 * Return the (named) transitions of an instance
 * @param array $instance
 */
function _wf_field_get_transition_labels($instance) {
  $field = _wf_field_get_field_info($instance);  
  if (!isset($field['settings']['wf_settings']['wf_transitions'])) {
    return FALSE;
  }
  return $field['settings']['wf_settings']['wf_transitions'];
}


/**
 * Return the (named) transitions of an instance
 */
function _wf_field_has_transition_label($from, $to, $instance) {
  $field = _wf_field_get_field_info($instance);
  if (!isset($field['settings']['wf_settings']['wf_transitions'][$from . '_' . $to])) {
    return NULL;
  } 
  elseif (!$field['settings']['wf_settings']['wf_transitions'][$from . '_' . $to]) {
    return FALSE;
  }
  else {
    return $field['settings']['wf_settings']['wf_transitions'][$from . '_' . $to];
  }
}

/**
 * Helper function that returns all fields of type wf_fields (optionally those attach
 * @param string $entity_type
 * @param string $bundle_name
 * @param string $field_name
 * @return array of instances
 */
function _wf_field_get_all_fields($entity_type=NULL, $bundle_name=NULL) {
  $fields = field_info_fields();
  $wf_fields = array();
  foreach ($fields as $field_name => $field_info) {
    if ($field_info['module'] == 'wf_field') {
      // filter by entity type
      if ( isset($entity_type) && !isset($bundle_name) ) {
        $include = isset($field_info['bundles'][$entity_type]);
      }
      // filter by entity type and bundle name 
      elseif ( isset($entity_type) && isset($bundle_name) ) {
        $include = isset($field_info['bundles'][$entity_type]) && in_array($bundle_name, $field_info['bundles'][$entity_type]);
      }
      // otherwise include
      else {
        $include = true;
      }
      if ($include) {
        $wf_fields[$field_info['field_name']] = $field_info;
      }
    }
  }
  return $wf_fields;
}

/**
 * Helper function that returns all instances wf_fields
 * @param string $entity_type
 * @param string $bundle_name
 * @param string $field_name
 * @return array of instances
 */
function _wf_field_get_all_instances($entity_type=NULL, $bundle_name=NULL, $field_name=NULL) {
  $instances = array();
  if (!isset($entity_type) && !isset($bundle_name)) {
    $v = field_info_instances($entity_type, $bundle_name);
    foreach ($v as $entity_type => $bundles) {
      foreach ($bundles as $bundle) {
        foreach ($bundle as $instance) {
          $field = field_info_field_by_id($instance['field_id']);
          // @todo: there must be a way to filter this?
          if (isset($field['settings']['wf_settings'])) {
            $instances[] = $instance;
          }
        }
      }
    }
  }
  elseif ($entity_type && !$bundle_name) {
    $bundles = field_info_instances($entity_type);
    foreach ($bundles as $bundle) {
      foreach ($bundle as $instance) {
        $field = field_info_field_by_id($instance['field_id']);
        // @todo: there must be a way to filter this?
        if (isset($field['settings']['wf_settings'])) {
          $instances[] = $instance;
        }
      }
    }
  }
  elseif ($entity_type && $bundle_name) {
    $bundle = field_info_instances($entity_type, $bundle_name);
    foreach ($bundle as $instance) {
      $field = field_info_field_by_id($instance['field_id']);
      // @todo: there must be a way to filter this?
      if (isset($field['settings']['wf_settings'])) {
        $instances[] = $instance;
      }
    }
  }  
  return $instances;
}

/**
 * Return a single field-instance by entity-type, field_name and bundle
 * @param unknown_type $entity_type
 * @param unknown_type $field_name
 * @param unknown_type $bundle_name
 */
function _wf_field_get_wf_field_instance($entity_type, $field_name, $bundle_name) {
  // hmm really the api is kinda weird w/ the order 
  $info = field_info_instance($entity_type, $field_name, $bundle_name);
  $field = field_info_field($info['field_name']);
  // this is a little whack; but we'll complete refactoring to field from instance in due course. 
  return !isset($field['settings']['wf_settings']) ? NULL : $info;
}


/**
 * Helper function for Workflow Fields listing state transitions for a field.
 * @param Array $field
 *   An array with field information, in the form returned by content_fields().
 * @return Array
 *   An array with possible workflow state transitions for the field. Values
 *   are the names of to and from states, as named in the allowed values list.
 *   Keyes are the number of the states, separated by underscore. An example
 *   return could be array('1_2' => 'needs review -> approved').
 */
function wf_field_get_transitions($state_or_node, $field_name_or_instance, $options=array() ) {
  // if state_or_node -> 
  if ( is_array($field_name_or_instance) ) {
    // if we have a value given, use this as a filter 
    if (isset($state_or_node)) {
      if (!is_object($state_or_node)) {
        $options['filter_from_state'] = $state_or_node;
      } 
      elseif (isset($field_name_or_instance['field_name'])) {
        // find the appropriate instance-field-value
        $field_name = $field_name_or_instance['field_name']; 
        $options['filter_from_state'] = isset($state_or_node->{$field_name}['und'][0]['wf_state']) ? $state_or_node->{$field_name}['und'][0]['wf_state'] : NULL;
      }       
    }
    $instance = $field_name_or_instance;
  } 
  elseif ( is_string($field_name_or_instance) && is_object($state_or_node) ) {
    // @todo - not just handle node.
    $instance = _wf_field_get_wf_field_instance('node', $field_name_or_instance, $state_or_node->type);
    if (!is_array($instance)) {
      return FALSE;
    }
    $field_name = $instance['field_name']; 
    $options['filter_from_state'] = isset($state_or_node->{$field_name}['und'][0]['wf_state']) ? $state_or_node->{$field_name}['und'][0]['wf_state'] : NULL;
  } 
  else {
    // we cannot determine the instance
    return FALSE;
  }
  
  // retrieve available states
  $states = _wf_field_get_states($instance);
  if (!is_array($states)) {
    return FALSE;
  }
  
  // filter by current state
  $filter_from_state=isset($options['filter_from_state']) ? $options['filter_from_state'] : NULL;

  $transition_labels = _wf_field_get_transition_labels($instance);

  // Build an array of the transitions, in the form {TRANSITION_ID}=>{TRANSITION_NAME} ("1_2"=>"Publish") 
  // or if we have filter_from_state given {TARGET_STATE}=>{TRANSITION_NAME} (2=>"Publish")    
  $transitions = array();

  if (isset($filter_from_state) && isset($options['include_no_change_option']) && $options['include_no_change_option']) {
    // Change transition name on current state to 'no changes'
    $transitions[$filter_from_state] = t('No changes');
  }

  // show all / do not hide empty transitions
  $all_transitions = isset($options['all_transitions']) && $options['all_transitions'];
  $no_transition_labels =  isset($options['no_transition_labels']) && $options['no_transition_labels'];
  $ignore_permissions = isset($options['ignore_permissions']) && $options['ignore_permissions'];
  $complete_info = isset($options['complete_info']) && $options['complete_info'];
  
  // how to key the returned array (by target-state-id or transition-id) 
  $use_transition_as_key = !isset($filter_from_state);
  
  // allow for checking for a different user  
  $account = isset($options['account']) ? $options['account'] : $GLOBALS['user'];

  // @todo: clean this up, this is a crawling mess ...
  foreach ($states as $from_number => $from_state) {
    foreach ($states as $to_number => $to_state) {
      // Exclude the case where from and to state are the same.
      if ( $from_number != $to_number && ( $all_transitions || _wf_field_has_transition_label($from_number, $to_number, $instance) ) ) {
        $label = _wf_field_has_transition_label($from_number, $to_number, $instance);
        $default_label = t('@from_state -> @to_state', array('@from_state' => $from_state, '@to_state' => $to_state));
        if ( !$label || $no_transition_labels ) {
          $label = $default_label; 
        }
        $key = 'wf_field ' . $instance['field_name'] . ' ' . $from_number . '_' . $to_number;
        
        // sometimes we want the full info  
        if ($complete_info) {
          $default_label = t('@from_state -> @to_state', array('@from_state' => $from_state, '@to_state' => $to_state));
          $raw_label = _wf_field_has_transition_label($from_number, $to_number, $instance);
          $label = array(
            'label' => $label,
            'raw_label' => $raw_label,
            'default_label' => $default_label,
            'full_label' => ( $label != $default_label ) ? t('!raw_label (!label)', array('!raw_label' => $default_label, '!label' => $label)) : $raw_label, 
            'from_state' => $from_state,
            'to_state' => $to_state,
            'permission_key' => $key,
            // disabled transition when explictly "empty" description/label (not NULL!)
            'disabled' => isset($raw_label) && (!$raw_label),
          ); 
        }

        // check permissions either for this specific transition or all transtion
        if ( $ignore_permissions || user_access( $key, $account ) || user_access( 'wf_field ' . $instance['field_name'] . ' all', $account)) {
          if ($use_transition_as_key) {
            $transitions[ $from_number . '_' . $to_number] = $label;
          }
          elseif ( $filter_from_state == $from_number ) {
            $transitions[ $to_number ] = $label;
          }
        }
      }
    }
  }
  
  // if we have a concrete instance of a node - let's allow
  // for certain target states to be removed (or even added)  
  if (is_object($state_or_node)) {
    // the transitions-array will be keyed by targetstate  
    $context =  array('object' => $state_or_node, 'field_instance' => $instance, 'options' => $options) ;
    drupal_alter('wf_field_transitions_alter', $transitions, $context);
  }
  
  return $transitions;
}

/**
 * Returns the current state label
 * @param int/object $state either a value, or if a node/entity is given the value will be determined 
 * @param array/string $instance  field-instance-array or field_name-string
 * @param array $options   options (@see wf_field_get_transitions  
 */
function wf_field_get_current_state_label($state, $instance, $options=array()) {
  if (is_string($instance) && is_object($state) ) {
    // @todo - not just handle node.
    $instance = _wf_field_get_wf_field_instance('node', $instance, $state->type);
    if (!is_array($instance)) {
      return FALSE;
    }
  }
  
  if (is_object($state)) {
    $field_name = $instance['field_name'];
    $state = $state->{$field_name}['und'][0]['wf_state'];
  } 
  $states = _wf_field_get_states($instance, $options);
  return isset($states[$state]) ? $states[$state] : t('-');
}
/**
 * 
 * Return a associative array (transition id => transition label) with all available  
 * @param int/object $state either a value, or if a node/entity is given the value will be determined 
 * @param array/string $instance  field-instance-array or field_name
 * @param unknown_type $options	options (@see wf_field_get_transitions)
 */
function wf_field_get_current_transition_options($state, $instance, $options=array()) {
  return wf_field_get_transitions( $state, $instance, $options );
}

/**
 * Check whether a transition is allowed
 * 
 * @param @param int/object $state either a value, or if a node/entity is given the value will be determined 
 * @param unknown_type $state/or $object
 * @param unknown_type $target_state
 * @param unknown_type $account
 */
function wf_field_is_transition_allowed($state, $instance, $target_state, $account=NULL) {
  $transition_options = wf_field_get_current_transition_options($state, $instance, array('account' => $account));
  return isset($transition_options[$target_state]);
}

/**
 * Programatically execute transition
 * @param object $node
 * @param string $field_name
 * @param string $target_state
 * @param array $options
 */ 
function wf_field_transition_execute($node, $field_name, $target_state, $options=array() ) {
  // @todo: better var handling 
  // $node -> entity
  // $field_name -> field_name or field-instance-array
  // $target_state -> target_state or transition_id?
  
  // referring to valid field instance?
  $instance = _wf_field_get_wf_field_instance('node', $field_name, $node->type);
  if (!$instance) {
    // return FALSE, if we can't find an appropriate field_instance
    return FALSE;
  }
  
  // is transition allowed? 
  // @todo: execute a transition as a different user (even user 1?) and pass that into this.
  if (!wf_field_is_transition_allowed($node, $instance, $target_state)) {
    return FALSE;
  }
  
  // @todo: check whether we want to have a new revision in that case: $node->revision = 1;
  $node->{$field_name}['und'][0]['wf_state'] = $target_state;
  try {
    node_save($node);
    return TRUE; 
  } catch (Exception $e) {
    return FALSE;
  }
}


function _wf_field_get_all_grouped_transitions($entity, $entity_type, $bundle_name, $field_name=NULL) {
  $instances = _wf_field_get_all_instances($entity_type, $bundle_name);
  // dpm(array('entity'=>$entity, 'entity_type' => $entity_type, 'bundle_name'=>$bundle_name, 'instances'=>$instances) );
  $grouped_transitions = array();
  foreach ($instances as $k => $instance) {
    if (!isset($field_name) || $field_name == $instance['field_name']) {
      $trans = wf_field_get_current_transition_options($entity, $instance);
      if (sizeof($trans)) {
        $grouped_transitions[$k]['transitions'] = $trans;
        $grouped_transitions[$k]['instance'] = $instance;
      }
    } 
  }
  return $grouped_transitions;
}

/**
 * Returns an array with all events emitted for all instances of wf_field for a given entity_type
 *  
 * @see also @file wf_field.rules.inc 
 * 
 * @param unknown_type $entity_type
 * @param unknown_type $module
 * @param unknown_type $variables
 */
function _wf_field_build_entity_transtion_rules($entity_type, $module, $variables=array() ) {
  $field_instances = _wf_field_get_all_instances( $entity_type );
  // Build up all the possible events for the workflow fields.
  $events = array();
  foreach ($field_instances as $instance) {
    $transitions = wf_field_get_transitions( NULL, $instance, array('all_transitions' => TRUE, 'ignore_permissions' => TRUE, 'complete_info' => TRUE) );
    foreach ($transitions as $key => $transition) {
      if (!$transition['disabled']) {
        $events['wf_field_transition_' . $entity_type . '_' . $instance['field_name'] . '_' . $key] = array(
          'group' => t('Workflow Field'),
          'label' => t('@entity_type: @field - Transition: !transition', array('@entity_type' => $entity_type, '@field' => $instance['field_name'], '!transition' => $transition['full_label'])),
          'module' => $module,
          'variables' => $variables
        );
      }
    }
    // on entering & on leaving states
    $states = _wf_field_get_states($instance);
    foreach ($states as $key => $label ) {
      $events['wf_field_in_' . $entity_type . '_' . $instance['field_name'] . '_' . $key] = array(
        'group' => t('Workflow Field'),
        'label' => t('@entity_type: @field - On entering state (=inward): !label', array('@entity_type' => $entity_type, '@field' => $instance['field_name'], '!label' => $label)),
        'module' => $module,
        'variables' => $variables
      );
      $events['wf_field_out_' . $entity_type . '_' . $instance['field_name'] . '_' . $key] = array(
        'group' => t('Workflow Field'),
        'label' => t('@entity_type: @field - On leaving state (=outward): !label', array('@entity_type' => $entity_type, '@field' => $instance['field_name'], '!label' => $label)),
        'module' => $module,
        'variables' => $variables
      );
    }
    // one rules event that will be triggered on "any" transition
    $events['wf_field_any_' . $entity_type . '_' . $instance['field_name'] ] = array(
      'group' => t('Workflow Field'),
      'label' => t('@entity_type: @field - on any transition', array('@entity_type' => $entity_type, '@field' => $instance['field_name'])),
      'module' => $module,
      'variables' => $variables
    );
  }
  return $events;
}


function _wf_field_get_current_workflow_state($entity, $field_name_or_instance) {
  if ( is_array($field_name_or_instance) ) {
    $field_name = $field_name_or_instance['field_name'];
  } 
  else {
    $field_name = $field_name_or_instance;
  }
  return isset($entity->{$field_name}['und'][0]['wf_state']) ? $entity->{$field_name}['und'][0]['wf_state'] : NULL;
}