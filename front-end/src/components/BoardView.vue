<template>
  <div class="board" tabindex="1">
    <div v-for="(r_item, i) in board.cells" :key="i">
      <cell v-for="(c_item, i) in r_item" :key="'A' + i"></cell>
    </div>
    <tile-view v-for="(tile, i) in tiles" :key="'B' + i" :tile="tile"></tile-view>
    <game-end-overlay :board="board" @gameRestarted="restartGame"></game-end-overlay>
  </div>
</template>

<script>
import Cell from "./Cell.vue";
import TileView from "./TileView.vue";
import GameEndOverlay from "./GameEndOverlay.vue";
import { Board } from "../board";

export default {
  components: {
    Cell,
    TileView,
    GameEndOverlay
  },

  data() {
    return {
      board: new Board()
    };
  },

  mounted() {
    this.registerCallbacks();
  },

  beforeDestroy() {
    this.deregisterCallbacks();
  },

  computed: {
    tiles() {
      return this.board.tiles.filter(tile => tile.value != 0);
    }
  },

  methods: {
    registerCallbacks() {
      window.addEventListener("keydown", this.handleKeyDown);
    },

    deregisterCallbacks() {
      window.removeEventListener("keydown", this.handleKeyDown);
    },

    handleKeyDown(event) {
      if (!this.board.hasWon() && event.keyCode >= 37 && event.keyCode <= 40) {
        event.preventDefault();
        this.board.move(event.keyCode - 37);
        this.$emit("boardUpdated");
      }
    },

    restartGame() {
      this.board = new Board();
    }
  }
};
</script>
