<template>
  <span :class="classes">{{tile.value}}</span>
</template>

<script>
export default {
  props: {
    tile: {
      type: Object,
      required: true
    }
  },
  computed: {
    classes() {
      const tile = this.tile;
      const classArray = ["tile", `tile${this.tile.value}`];

      if (!tile.mergedInto) {
        classArray.push(`position_${tile.row}_${tile.column}`);
      }
      if (tile.mergedInto) {
        classArray.push("merged");
      }
      if (tile.isNew()) {
        classArray.push("new");
      }
      if (tile.hasMoved()) {
        classArray.push("isMoving");
        classArray.push(`row_from_${tile.fromRow()}_to_${tile.toRow()}`);
        classArray.push(
          `column_from_${tile.fromColumn()}_to_${tile.toColumn()}`
        );
      }
      return classArray.join(" ");
    }
  }
};
</script>
