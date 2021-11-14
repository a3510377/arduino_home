import { defineConfig } from 'vite'
import { config } from 'dotenv'
import WindiCSS from 'vite-plugin-windicss'
import vue from '@vitejs/plugin-vue'
import path from 'path'
config()

// https://vitejs.dev/config/
export default defineConfig({
  base: process.env.NODE_ENV === 'development' ? '/' : process.env.VITE_BASE,
  plugins: [vue(), WindiCSS()],
  resolve: {
    alias: [{ find: '@', replacement: path.resolve(__dirname, './src') }],
  },
})
