<?php
// $Id$
/**
  * @file
  *
  * linkimagefield widget code
  * @author John Young <john@codeandcreative.com>
  */

/**
 * Implementation of CCK's hook_widget_settings($op = 'form')
 *
 * @param array $widget
 * @return array
 */
function linkimagefield_widget_settings_form($widget) {
  $form = module_invoke('imagefield', 'widget_settings', 'form', $widget);
  
  $form['url'] = array(
    '#type'          => 'textfield',
    '#maxlength'     => '255',
    '#title'         => t('Default URL'),
    '#description'   => t('Provide a well-formed URL.  This will be the default url linked to by provided images.'),
    '#default_value' => !empty($widget['url']) ? $widget['url'] : '',
  );
  
  $target_options = _linkimagefield_widget_url_target_options();
  
  $form['target'] = array(
    '#type'          => 'select',
    '#maxlength'     => '255',
    '#title'         => t('Default Target'),
    '#description'   => t('Select a default target type.'),
    '#default_value' => !empty($widget['target']) ? $widget['target'] : '_self',
  	'#options'			 => $target_options,
  );
  
  return $form;
}

/**
 * Implementation of CCK's hook_widget_settings($op = 'save').
 *
 * @param array $widget
 * @return array
 */
function linkimagefield_widget_settings_save($widget) {
  $imagefield_settings = module_invoke('imagefield', 'widget_settings', 'save', $widget);
  return array_merge($imagefield_settings, array('url', 'target'));
}

/**
 * Implementation of CCK's hook_widget_settings($op = 'validate').
 *
 * @param array $widget
 */
function linkimagefield_widget_settings_validate($widget) {  
  module_invoke('imagefield', 'widget_settings', 'validate', $widget);
  
  // Check the URL
  if (!empty($widget['url'])) {
    if (!valid_url($widget['url'])) {
      form_set_error('url', t('Please provide a valid url.'));
    } else {
      $widget['url'] = check_url($widget['url']);
    }
  }
  
  // Check the target
  if (!empty($widget['target'])) {
    $options = array('_self', '_blank', '_parent', '_top');
    
    if(!in_array($widget['target'], $options)) {
      $widget['target'] = '_self';
    }
  }
}

/**
 * Element #value_callback function.
 *
 * @param array $element
 * @param mixed $edit
 * @return array
 */
function linkimagefield_widget_value($element, $edit = FALSE) {
  $item = imagefield_widget_value($element, $edit);
  if ($edit) {
    $item['url'] = isset($edit['url']) ? $edit['url'] : '';
    $item['target'] = isset($edit['target']) ? $edit['target'] : '_self';
  }
  else {
    $item['url'] = '';
    $item['target'] = '_self';
  }
  return $item;
}

/**
 * Element #process callback function.
 *
 * @param array $element
 * @param array $edit
 * @param arrayreference $form_state
 * @param array $form
 * @return array
 */
function linkimagefield_widget_process($element, $edit, &$form_state, $form) {
  $file = $element['#value'];
  $element = imagefield_widget_process($element, $edit, &$form_state, $form);

  $field = content_fields($element['#field_name'], $element['#type_name']);
  $element['#theme'] = 'linkimagefield_widget_item';
  
  $element['data']['url'] = array(
    '#title' => t('URL'),
    '#type' => 'textfield',
    '#default_value' => $file['data']['url'] ? $file['data']['url'] : $field['widget']['url'],
    '#description' => t('This URL will be loaded when the image is clicked'),
    '#maxlength' => 255,
  );
  
  $element['data']['target'] = array(
    '#title'         => t('Target'),
    '#type'          => 'select',
    '#default_value' => $file['data']['target'] ? $file['data']['target'] : $field['widget']['target'],
    '#description'   => t('Window/Frame Target for the URL'),
    '#options' 		   => _linkimagefield_widget_url_target_options(),
  );
  return $element;
}

/**
 * FormAPI theme function. Theme the output of an image field.
 *
 * @param arrayreference $element
 * @return string
 */
function theme_linkimagefield_widget(&$element) {
  drupal_add_css(drupal_get_path('module', 'imagefield') .'/imagefield.css');
  return theme('form_element', $element, $element['#children']);
}

/**
 * the list of URL frame targets
 *
 * @return array
 */
function _linkimagefield_widget_url_target_options(){
  return array(
    '_self'				  => t('Same Window (_self)'),
    '_blank'				=> t('New Window (_blank)'),
    '_parent'				=> t('Parent Frame (_parent)'),
    '_top'				  => t('Top Frame (_top)'),
  );
}