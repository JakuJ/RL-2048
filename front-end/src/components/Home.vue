<template>
  <div>
    <h1 class="title">Play 2048</h1>
    <board-view ref="boardView"></board-view>
    <button class="defaultButton" @click="botMode">Let bot play</button>
  </div>
</template>

<script>
import BoardView from "./BoardView.vue";
export default {
  components: { BoardView },
  methods: {
    botMode() {
      this.$refs.boardView.deregisterCallbacks();
      // TODO: Tutaj podpiąć bota
    }
  }
};
</script>

<style scoped>
</style>