<?php
// $Id$
/**
 * @file
 * 
 * Administrative page callbacks.
 */
function wf_field_admin_settings() {
  // @todo: isn't this a block configuration? but me hateth blocks, strongarming me prefer.
  $form['wf_field_trigger_transition_block_use_confirmation_form'] = array(
    '#title' => t('Trigger transition block'),
    '#type' => 'select',
    '#options' => array( 0 => t('Use links'), 1 => t('Use confirmation form') ),
    '#default_value' => variable_get('wf_field_trigger_transition_block_use_confirmation_form', FALSE),
  );
  return system_settings_form($form);
}