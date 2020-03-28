<template>
  <div class="overlay" v-show="show">
    <p class="message">{{contents}}</p>
    <button class="defaultButton" @click="restart">Try again</button>
  </div>
</template>

<script>
export default {
  props: {
    board: {
      type: Object,
      required: true
    }
  },
  computed: {
    show() {
      return this.board.hasWon() || this.board.hasLost();
    },
    contents() {
      if (this.board.hasWon()) {
        return "Good Job!";
      } else if (this.board.hasLost()) {
        return "Game Over";
      }
    }
  },

  methods: {
    restart() {
      this.$emit("gameRestarted");
    }
  }
};
</script>
