<?php
// $Id$

/**
 * @file
 * Administration page callbacks for the annotate module.
 */

/**
 * Form builder. Configure annotations.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function annotate_admin_settings() {

  // Get an array of node types with internal names as keys and
  // "friendly names" as values. E.g.,
  // array('page' => 'Page', 'story' => 'Story')
  
  $types = node_type_get_types();
  foreach($types as $node_type) { 
     $options[$node_type->type] = $node_type->name;
  }  
  
  $form['annotate_node_types'] = array( 
    '#type' => 'checkboxes', 
    '#title' => t('Users may annotate these content types'), 
    '#options' => $options, 
    '#default_value' => variable_get('annotate_node_types', array('page')), 
    '#description' => t('A text field will be available on these content types to
       make user-specific notes.'), 
  ); 
  
  $form['annotate_deletion'] = array(
    '#type' => 'radios',
    '#title' => t('Annotations will be deleted'),
    '#description' => t('Select a method for deleting annotations.'),
    '#options' => array(
      t('Never'),
      t('Randomly'),
      t('After 30 days')
    ),
    '#default_value' => variable_get('annotate_deletion', 0) // Default to Never
  );

  $form['annotate_limit_per_node'] = array(
    '#type' => 'textfield',
    '#title' => t('Annotations per node'),
    '#description' => t('Enter the maximum number of annotations allowed per
      node (0 for no limit).'),
    '#default_value' => variable_get('annotate_limit_per_node', 1),
    '#size' => 3
  );

  
  

  return system_settings_form($form, TRUE); 

}

/**
 * annotate_admin_settings_validate
 *
 * Update content types to reflect changes to the annotation configuration options selected by the site administrator. 
 * If the administrator checked a box for a content type that did not previously have an annotation field then add it to the 
 * content type.   If a content type is unchecked by the administrator and that content type had an annotation field, then 
 * remove that field from the content type.
 */
function annotate_admin_settings_validate($form, $form_state) {

  $limit = $form_state['values']['annotate_limit_per_node'];
  if (!is_numeric($limit)) {
    form_set_error('annotate_limit_per_node', t('Please enter a number.'));
    return;
  }
 
// loop through each of the content type checkboxes shown on the form 
  foreach ($form_state['values']['annotate_node_types'] as $key => $value) { 

// if the check box for a content type is unchecked, look to see whether this content type has the 
// annotation field attached to it using the field_info_instance function.  If it does then we 
// need to remove the annotation field as the administrator has unchecked the box.
     if (!$value) { 
        $instance = field_info_instance('node', 'annotation', $key);
     	if (!empty($instance)) {
           field_delete_instance($instance);     
           watchdog("Annotation", "Deleted annotation field from content type: ".$key);
        } // end of annotation existed, need to remove it
     } else { 
// if the check box is checked for a content type look to see whether the field is associated with that 
// content type.  If not then add the annotation field to the content type
        $instance = field_info_instance('node', 'annotation', $key);
    	if (empty($instance)) {
	    	$instance = array(
    		 'field_name' => 'annotation',
      		 'entity_type' => 'node',
      		 'bundle' => $key,
      	 	 'label' => t('Annotation'),
      	 	 'widget_type' => 'text_textarea_with_summary',
      	 	 'settings' => array('display_summary' => TRUE),
      	 	 'display' => array(
        	    'default' => array(
        	  	   'type' => 'text_default',
    	         ),
	            'teaser' => array(
          		   'type' => 'text_summary_or_trimmed',
        	     ),
      	      ),
            );
    	
            $instance = field_create_instance($instance);
    
            watchdog("Annotation", "Added annotation field to content type: ".$key);
        } // end of annotation field didn't exist so add it
     } // end of if
  
  } // end of foreach


} 





