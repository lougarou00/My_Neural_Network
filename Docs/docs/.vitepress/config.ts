import { defineConfig } from 'vitepress';

// see https://vitepress.dev/reference/site-config for details
export default defineConfig({
  lang: 'en-US',
  title: 'MY_TOUCH - 2k25',
  base: process.env.NODE_ENV === 'production' ? '/G-CPP-500-COT-5-1-rtype-22/Docs/' : '/',
  description: 'MY_TOUCH project documentation',
  
  head: [
    ['link', { rel: 'preconnect', href: 'https://fonts.googleapis.com' }],
    ['link', { rel: 'preconnect', href: 'https://fonts.gstatic.com', crossorigin: '' }],
    ['link', { href: 'https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700;800;900&display=swap', rel: 'stylesheet' }],
    ['script', { src: 'https://code.iconify.design/iconify-icon/1.0.8/iconify-icon.min.js' }],
    ['meta', { name: 'theme-color', content: '#1100ffff' }],
  ],

  themeConfig: {
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Our Team', link: '/TEAM_ORGANIZATION' },
      {
        text: 'Documentation',
        items: [
          { text: 'Architecture', link: '/Architecture/ARCHITECTURE' },
          { text: 'Technical Study', link: '/Technical/TECHNICAL_DOCUMENTATION' },
          { text: 'Developer Guide', link: '/Developer/CONTRIBUTING' },
          { text: 'User Guide', link: '/Developer/USER_GUIDE' },
          { text: 'Build & Deploy', link: '/Build/BUILD_GUIDE' },
          { text: 'Project Details', link: '/MY_TOUCH' },
        ]
      },
    ],

    sidebar: [
      {
        text: 'Overview',
        items: [
          { text: 'MY_TOUCH Project', link: '/' },
          { text: 'Our Team', link: '/TEAM_ORGANIZATION' },
        ],
      },
      {
        text: 'Architecture',
        collapsed: false,
        items: [
          { text: 'System Architecture', link: '/Architecture/ARCHITECTURE' },
        ],
      },
      {
        text: 'Technical documentation',
        collapsed: true,
        items: [
          { text: 'Technical documentation', link: '/Technical/TECHNICAL_DOCUMENTATION' },
          { text: 'FEN to Neural Network Input Encoding Specification', link: '/Technical/how_to_convert' },
          { text: 'Hyperparameters saving', link: '/Technical/hyperparameters_saving' },
        ],
      },
      {
        text: 'Benchmarking',
        collapsed: false,
        items: [
          { text: 'Benchmark data convention', link: '/Benchmark/benchmark_JSON_Convention' },
          { text: 'Benchmark structure', link: '/Benchmark/benchmark_structure' },
          { text: 'Benchmark technical documentation', link: '/Benchmark/benchmark_technical_documentation' },
          { text: 'Run and visualize benchmark', link: '/Benchmark/startBenchMark' },
        ],
      },
      {
        text: 'Developer & User Guide',
        collapsed: true,
        items: [
          { text: 'Developer guide', link: '/Developer/CONTRIBUTING' },
          { text: 'User guide', link: '/Developer/USER_GUIDE' },
        ],
      },
      {
        text: 'Build & Deploy',
        collapsed: true,
        items: [
          { text: 'Build Instructions', link: '/Build/BUILD_GUIDE' },
          { text: 'CI/CD Pipeline', link: '/Developer/contributing#cicd-pipeline' },
        ],
      },
    ],
  },

  // vite: {
  //   css: {
  //     preprocessorOptions: {
  //       css: {
  //         additionalData: '@import "./theme/custom.css";'
  //       }
  //     }
  //   }
  // },

  markdown: {
    theme: {
      light: 'github-light',
      dark: 'github-dark'
    },
    lineNumbers: true
  }
})
