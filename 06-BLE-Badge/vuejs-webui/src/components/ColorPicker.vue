<!--

Events: input is fired on every color change in the picker, change is fired when the user dismisses the picker.

-->

<template>
  <div class="">
    <input type="color" value="#e66465" v-model="hexColor">
  </div>
</template>

<script>

export default {
  name: 'ColorPicker',
  props: ['color'],
  data() {
    return {
      hexColor: '',
    }
  },
  watch: {
    color: function (value, oldValue) { // 255,3,4
      if (value === oldValue) {
        return;
      }
      let p = value.split(',');
      this.hexColor = rgbToHex(Number(p[0]), Number(p[1]), Number(p[2]));
    },
    hexColor: function (value, oldValue) {
      let rgb = hexToRgb(value)
      this.$emit('update:color', rgb.r + ',' + rgb.g + ',' + rgb.b);
    }
  },
  computed: {
  },
  methods: {
  }
}

function componentToHex(c) {
  var hex = c.toString(16);
  return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
  return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function hexToRgb(hex) {
  var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result ? {
    r: parseInt(result[1], 16),
    g: parseInt(result[2], 16),
    b: parseInt(result[3], 16)
  } : null;
}

</script>

<style scoped>

</style>
