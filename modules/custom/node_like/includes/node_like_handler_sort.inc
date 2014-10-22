<?php

/**
 * @file
 * Contains the class to extend views by providing a new custom sort param
 * by count node likes for speciffic CTs.
 */

/**
 * @defgroup views_sort_handlers Views sort handlers
 * @{
 * Handlers to tell Views how to sort queries.
 */

/**
 * Base sort handler that has no options and performs a simple sort.
 *
 * @ingroup views_sort_handlers
 */
class node_like_handler_sort extends views_handler_sort {
  /**
   * Called to add the field to a query.
   */
  function query() {
    // Get all view fields and check if field "likes" is present. If not then alter
    // query in the same logic as field handler.
    $fields = $this->query->fields;
    if (!in_array($this->field, $fields)) {
      $this->ensure_my_table();

      // Add the field.
      $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
      // Get base table name.
      $base_table = $this->query->base_table;
      // Get field formula
      $formula = $this->get_formula('node_like', $base_table);
      $this->field_alias = $this->query->add_field(null, $formula, $this->field, $params);
      $this->query->distinct = TRUE;
    }
    // Add condition.
    $this->query->add_orderby(NULL, NULL, $this->options['order'], $formula);
  }

  /**
   * Add formula for getting field data.
   */
  function get_formula($join_table, $base_table) {
    return "(SELECT COUNT($join_table.nid) FROM $join_table WHERE $join_table.nid = $base_table.nid GROUP BY $join_table.nid)";
  }
}
