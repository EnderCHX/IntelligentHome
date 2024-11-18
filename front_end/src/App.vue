<script setup>
import IconCHX from './components/icons/IconCHX.vue'
import { ref } from 'vue'
import { darkTheme, lightTheme } from 'naive-ui'

import hljs from 'highlight.js/lib/core'
import javascript from 'highlight.js/lib/languages/javascript'
import golang from 'highlight.js/lib/languages/go'
import cpp from 'highlight.js/lib/languages/cpp'

hljs.registerLanguage('javascript', javascript)
hljs.registerLanguage('go', golang)
hljs.registerLanguage('cpp', cpp)

const isDark = ref(false);
const theme = ref(lightTheme);
const title = ref('智能家居');

const toggleTheme = (value) => {
  isDark.value = value;
  theme.value = value ? darkTheme : lightTheme;
};

</script>

<template>
  <n-config-provider :theme="theme" :hljs="hljs">

    <n-layout style="height: 100vh; width: 100vw;">

      <n-back-top :right="100" />

      <n-layout-header class="header">

        <n-space justify="center">
          <router-link to="/">
            <n-button text>
              {{ title }}
            </n-button>
          </router-link>
          <n-divider vertical />

          <router-link to="/">
            <n-button text :render-icon="renderIcon"><n-icon>
                <Home />
              </n-icon>Home</n-button>
          </router-link>
          <n-divider vertical />

          <router-link to="/history">
            <n-button text :render-icon="renderIcon"><n-icon>
                <IconCHX />
              </n-icon>温湿度曲线图</n-button>
          </router-link>
          <n-divider vertical />

          <router-link to="/about">
            <n-button text :render-icon="renderIcon"><n-icon>
                <IconCHX />
              </n-icon>About</n-button>
          </router-link>
          <n-divider vertical />

          <n-switch :rail-style="railStyle" :value="isDark" @update:value="toggleTheme">
            <template #checked>
              暗色主题
            </template>
            <template #unchecked>
              亮色主题
            </template>
          </n-switch>
          <n-divider vertical />
        </n-space>

      </n-layout-header>

      <n-layout-content class="content">
        
        <RouterView />
      </n-layout-content>
    </n-layout>
  </n-config-provider>
</template>

<style scoped>
.header {
  padding: 2vh;
  position: fixed;
  width: 100%;
  height: 8vh;
  line-height: 6vh;
  z-index: 10;
}

.content {
  margin-top: 8vh;
  padding: 1vh;
}
</style>