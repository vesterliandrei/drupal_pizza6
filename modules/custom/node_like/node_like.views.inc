<?php

/**
 * @file
 * This file provides custom field handlers for Views from Node Like module.
 */

/**
 * Implementation of hook_views_data().
 */
function node_like_views_data() {
   // Add Count of node likes field/filter/argument/sort param.
  $data['node']['likes'] = array(
    'title' => t('Node likes count'),
    'group' => t('Node like'),
    'help' => t('Return number of node likes for current node.'),
    'field' => array(
      'handler' => 'node_like_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'node_like_handler_sort',
    ),
    'filter' => array(
      'handler' => 'node_like_handler_filter',
    ),
    'real field' => 'likes',
  );

  return $data;
}
