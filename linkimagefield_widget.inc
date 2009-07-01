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
  return array_merge($imagefield_settings, array('default_url'));
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
      return;
    }
    
    $widget['url'] = check_url($widget['url']);
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
  }
  else {
    $item['url'] = '';
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

  $default_url = (empty($file['status']) && empty($file['data']['url']));
  $field = content_fields($element['#field_name'], $element['#type_name']);
  $element['#theme'] = 'linkimagefield_widget_item';
  
  $element['data']['url'] = array(
    '#title' => t('URL'),
    '#type' => 'textfield',
    '#default_value' => $default_alt ? $field['widget']['url'] : $file['data']['url'],
    '#description' => t('This URL will be loaded when the image is clicked'),
    '#maxlength' => 255,
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