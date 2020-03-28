<template>
  <div>
    <h1 class="title centered">Play 2048</h1>
    <p class="centered">Score: {{ score }}</p>
    <board-view ref="boardView" v-on:boardUpdated="updateScore"></board-view>
    <button id="botButton" class="defaultButton centered" @click="botMode">Let the bot play</button>
  </div>
</template>

<script>
import BoardView from "./BoardView.vue";
export default {
  components: { BoardView },
  data() {
    return {
      score: 0
    };
  },
  methods: {
    botMode() {
      this.$refs.boardView.deregisterCallbacks();
      // TODO: Tutaj podpiąć bota
    },
    updateScore() {
      this.score = this.$refs.boardView.board.score;
    }
  }
};
</script>

<style scoped>
.title {
  font-family: "Clear Sans";
}

.centered {
  display: block;
  margin: 10px auto;
  text-align: center;
}
#botButton {
  margin-top: 20px;
}
</style>
