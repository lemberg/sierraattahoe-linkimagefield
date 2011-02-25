<?php
/**
  * @file
  *
  * linkimagefield formatter code
  * @author John Young <john@codeandcreative.com>
  */
function theme_linkimagefield_formatter_linkimage($element) {
  // Inside a view $element may contain null data. In that case, just return.
  if (empty($element['#item']['fid'])) {
    return '';
  }

  $field = content_fields($element['#field_name']);
  $item = $element['#item'];

  $item['data']['alt'] = isset($item['data']['alt']) ? $item['data']['alt'] : '';
  $item['data']['title'] = isset($item['data']['title']) ? $item['data']['title'] : NULL;
  $item['data']['url'] = isset($item['data']['url']) ? $item['data']['url'] : NULL;

  $class = 'linkimagefield linkimagefield-'. $field['field_name'];
  return theme('linkimagefield_image', $item, $item['data']['alt'], $item['data']['title'], array('class' => $class), TRUE, $item['data']['url']);
}
