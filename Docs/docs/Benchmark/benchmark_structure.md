# Neural Network Benchmark Dashboard - Project Structure

```
neural-benchmark-dashboard/
│
├── public/                              # Static assets
│   └── benchmark-data.json             # Mock benchmark data (9 sample entries)
│
├── src/                                # Source code
│   ├── NeuralBenchmarkDashboard.jsx   # Main dashboard component (500+ lines)
│   ├── main.jsx                        # React application entry point
│   └── index.css                       # Tailwind CSS with custom styles
│
├── index.html                          # HTML template
├── package.json                        # Project dependencies and scripts
├── vite.config.js                      # Vite build configuration
├── tailwind.config.js                  # Tailwind CSS configuration
├── postcss.config.js                   # PostCSS configuration
├── .gitignore                          # Git ignore rules
├── README.md                           # Project documentation
└── TECHNICAL_DOCUMENTATION.md          # In-depth technical guide

```

## Component Breakdown

### NeuralBenchmarkDashboard.jsx Components

1. **NeuralBenchmarkDashboard** (Main)
   - Root component
   - State management
   - Data loading logic
   - Component orchestration

2. **ExperimentSelector**
   - Dropdown for experiment selection
   - Custom styled select element

3. **MetricCard**
   - Display individual metrics
   - Show trend indicators
   - Support multiple formats (percent, decimal)

4. **PerformanceChart**
   - Interactive line chart using Recharts
   - Multiple metric lines
   - Custom tooltips and legends
   - Clickable data points

5. **MetricToggle**
   - Toggle switches for metrics
   - Show/hide chart lines
   - Animated toggle buttons

6. **EpochDetailModal**
   - Full-screen modal overlay
   - Comprehensive epoch details
   - Performance metrics grid
   - Network architecture display

## Data Flow

```
JSON File → loadBenchmarkData() → benchmarkData (state)
                                        ↓
                            experimentIds (useMemo)
                                        ↓
                            ExperimentSelector
                                        ↓
                            selectedExperiment (state)
                                        ↓
                            experimentData (useMemo)
                                        ↓
                    ┌───────────────────┴───────────────────┐
                    ↓                                       ↓
            currentMetrics (useMemo)              PerformanceChart
                    ↓                                       ↓
                MetricCard                          selectedEpoch (state)
                                                            ↓
                                                    EpochDetailModal
```

## File Sizes (Approximate)

- NeuralBenchmarkDashboard.jsx: ~18KB
- benchmark-data.json: ~8KB
- index.css: ~1KB
- README.md: ~6KB
- TECHNICAL_DOCUMENTATION.md: ~12KB

## Dependencies

### Production
- react: ^18.2.0
- react-dom: ^18.2.0
- recharts: ^2.10.0

### Development
- vite: ^5.0.8
- @vitejs/plugin-react: ^4.2.0
- tailwindcss: ^3.4.0
- autoprefixer: ^10.4.16
- postcss: ^8.4.32

## Build Output Structure (after npm run build)

```
dist/
├── index.html
├── assets/
│   ├── index-[hash].js
│   └── index-[hash].css
└── benchmark-data.json
```

## Key Features by File

### NeuralBenchmarkDashboard.jsx
- 9 mock data entries across 3 experiments
- 6 React components
- Real-time metric calculations
- Interactive data point selection
- Responsive chart rendering
- Modal state management

### benchmark-data.json
- 3 unique experiments (exp_001, exp_002, exp_003)
- Multiple epochs per experiment
- Complete network configurations
- Performance metrics per epoch
- Timestamps and metadata

### index.css
- Tailwind base, components, utilities
- Custom font import (JetBrains Mono)
- Custom animation keyframes
- Dark theme variables

## Color Palette

```css
Background: #0c0a09 (stone-950)
Cards: #1c1917 (stone-900)
Borders: #44403c (stone-800)
Text: #f5f5f4 (stone-100)
Accent: #f59e0b (amber-500)
Muted: #78716c (stone-500)
```

## Typography

- Primary: JetBrains Mono (monospace)
- Weights: 400, 500, 700, 800
- Used for all text elements
- Technical, data-focused aesthetic

## Screen Support

- Desktop: 1440px+ (optimal)
- Laptop: 1024px+ (supported)
- Tablet: 768px+ (responsive)
- Mobile: Not optimized (dashboard is desktop-focused)