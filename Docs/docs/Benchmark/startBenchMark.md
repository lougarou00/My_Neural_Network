# Quick Start Guide

## Installation & Setup

1. **Install Dependencies**
   ```bash
   npm install
   ```

2. **Start Development Server**
   ```bash
   npm run dev
   ```
   The dashboard will open at `http://localhost:3000`

3. **Build for Production**
   ```bash
   npm run build
   ```
   Output will be in the `dist/` directory

## First Steps

1. **Select an Experiment**: Use the dropdown to choose from `exp_001`, `exp_002`, or `exp_003`

2. **View Metrics**: The dashboard displays:
   - Current accuracy, loss, and F1 score with trend indicators
   - Interactive time-series chart showing performance evolution

3. **Toggle Metrics**: Use the sidebar toggles to show/hide different metrics on the chart

4. **Inspect Epochs**: Click any data point on the chart to view detailed information about that specific training run

5. **Reload Data**: Click the "Reload Data" button to fetch updated benchmark results

## Mock Data Structure

The project includes 9 sample benchmark entries across 3 experiments:
- **exp_001**: 4 epochs (basic architecture)
- **exp_002**: 3 epochs (deeper network)
- **exp_003**: 2 epochs (lightweight architecture)

## Customization

### Adding Your Own Data

Replace the contents of `public/benchmark-data.json` with your actual benchmark results following the same JSON schema.

### Changing Colors

Edit the `colors` object in `src/NeuralBenchmarkDashboard.jsx`:
```javascript
const colors = {
  accuracy: '#f59e0b',  // Amber
  loss: '#ef4444',      // Red
  // Add your colors
};
```

### Adding New Metrics

1. Add to `activeMetrics` state
2. Add a new `<Line>` component in `PerformanceChart`
3. Update the color scheme

## Project Files

```
├── src/
│   ├── NeuralBenchmarkDashboard.jsx  # Main component with all sub-components
│   ├── main.jsx                       # React entry point
│   └── index.css                      # Styles with Tailwind
├── public/
│   └── benchmark-data.json            # Sample benchmark data
├── index.html                         # HTML template
├── package.json                       # Dependencies
├── vite.config.js                     # Build configuration
├── tailwind.config.js                 # Tailwind configuration
├── README.md                          # Full documentation
├── TECHNICAL_DOCUMENTATION.md         # Technical deep-dive
└── PROJECT_STRUCTURE.md               # Project organization
```

## Common Issues

### Port Already in Use
If port 3000 is busy, Vite will automatically try the next available port.

### Missing Dependencies
Run `npm install` to ensure all packages are installed.

### Chart Not Rendering
Verify that `recharts` is properly installed: `npm install recharts`

## Next Steps

1. Read `README.md` for comprehensive feature documentation
2. Read `TECHNICAL_DOCUMENTATION.md` for architecture details
3. Customize the mock data in `public/benchmark-data.json`
4. Modify components in `src/NeuralBenchmarkDashboard.jsx`

## Support

For issues or questions, refer to:
- README.md for feature documentation
- TECHNICAL_DOCUMENTATION.md for implementation details
- PROJECT_STRUCTURE.md for file organization