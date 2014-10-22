<?php

/**
 * @file
 * Contains the class to extend views by providing a new custom filter param
 * by count node likes for speciffic CTs.
 */

/**
 * Base class for filters.
 *
 * @ingroup views_filter_handlers
 */
class node_like_handler_filter extends views_handler_filter_numeric {
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
    // Add the HAVING condition.
    $this->query->add_having($this->options['group'], $this->field, end($this->value), $this->operator);
  }

  /**
   * Add formula for getting field data.
   */
  function get_formula($join_table, $base_table) {
    return "(SELECT COUNT($join_table.nid) FROM $join_table WHERE $join_table.nid = $base_table.nid GROUP BY $join_table.nid)";
  }
}
