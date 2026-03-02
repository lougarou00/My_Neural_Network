# Neural Network Benchmark Dashboard

A professional, production-ready dashboard for visualizing neural network performance metrics and training evolution over time.

## Features

- **Experiment Selection**: Choose from multiple experiments to analyze
- **Real-time Metrics**: Display current accuracy, loss, and F1 score with trend indicators
- **Interactive Charts**: Time-series visualization with clickable data points
- **Metric Toggle**: Show/hide different performance metrics on the chart
- **Epoch Details**: Click any data point to view comprehensive metrics and configuration
- **Data Reload**: Fetch updated benchmark data without page refresh
- **Responsive Design**: Professional UI optimized for desktop viewing

## Technology Stack

- **React 18**: Functional components with hooks
- **Recharts**: Professional charting library
- **Tailwind CSS**: Utility-first styling
- **Vite**: Fast build tool and dev server

## Project Structure

```
neural-benchmark-dashboard/
├── public/
│   └── benchmark-data.json       # Mock benchmark data
├── src/
│   ├── NeuralBenchmarkDashboard.jsx  # Main component
│   ├── main.jsx                  # React entry point
│   └── index.css                 # Tailwind CSS config
├── index.html                    # HTML template
├── package.json                  # Dependencies
├── vite.config.js               # Vite configuration
├── tailwind.config.js           # Tailwind configuration
└── postcss.config.js            # PostCSS configuration
```

## Installation

```bash
npm install
```

## Development

```bash
npm run dev
```

The application will open automatically at `http://localhost:3000`

## Build for Production

```bash
npm run build
```

## Data Format

The dashboard expects benchmark data in the following JSON format:

```json
{
  "experiment_id": "exp_001",
  "date": "2025-12-16T14:35:00Z",
  "network_config": {
    "num_layers": 4,
    "layers": [
      { "type": "input", "size": 784, "neuron_type": "perceptron" },
      { "type": "hidden", "size": 128, "neuron_type": "relu" }
    ],
    "learning_rate": 0.01,
    "batch_size": 32
  },
  "training_data_size": 60000,
  "performance": {
    "accuracy": 0.945,
    "loss": 0.21,
    "precision": 0.93,
    "recall": 0.92,
    "f1_score": 0.925,
    "training_duration_sec": 125.4,
    "multithreaded": true,
    "threads_used": 8
  },
  "timestamps": {
    "start_time": "2025-12-16T14:35:00Z",
    "end_time": "2025-12-16T14:37:05Z"
  },
  "notes": "Optional notes about this run"
}
```

## Component Architecture

### Main Components

1. **NeuralBenchmarkDashboard**: Root component managing state and data flow
2. **ExperimentSelector**: Dropdown for selecting experiments
3. **MetricCard**: Display individual performance metrics with trends
4. **PerformanceChart**: Interactive time-series chart using Recharts
5. **MetricToggle**: Controls for showing/hiding chart metrics
6. **EpochDetailModal**: Full-screen modal for detailed epoch information

### State Management

The application uses React hooks for state:
- `benchmarkData`: All loaded benchmark entries
- `selectedExperiment`: Currently selected experiment ID
- `selectedEpoch`: Epoch data for detail modal
- `activeMetrics`: Toggle state for visible metrics
- `isLoading`: Loading state for data fetching

## Key Features Explained

### Data Loading

The `loadBenchmarkData` function simulates fetching data from a JSON file. In production, replace the `MOCK_DATA` constant with an actual API call or file fetch.

### Experiment Grouping

Data is grouped by `experiment_id` using `useMemo`:
- Extracts unique experiment IDs
- Filters data for selected experiment
- Sorts entries chronologically by date

### Chart Updates

Charts update reactively when:
- A different experiment is selected
- Metrics are toggled on/off
- New data is loaded via the reload button

The `experimentData` memo automatically re-computes when dependencies change.

### Reload Mechanism

The reload button:
1. Sets loading state to true
2. Simulates data fetch (500ms delay)
3. Updates benchmark data
4. Automatically triggers chart re-render

In production, this would make an HTTP request to fetch updated JSON data.

### Interactive Data Points

Clicking a data point on the chart:
1. Triggers `onDataPointClick` handler
2. Sets `selectedEpoch` state
3. Opens modal with complete epoch details
4. Displays all metrics, configuration, and metadata

## Extending the Dashboard

### Adding New Metrics

To add a new metric to the charts:

1. Add it to the `activeMetrics` state:
```javascript
const [activeMetrics, setActiveMetrics] = useState({
  accuracy: true,
  loss: true,
  your_metric: false  // Add here
});
```

2. Add a Line component in PerformanceChart:
```javascript
<Line 
  type="monotone" 
  dataKey="performance.your_metric" 
  stroke="#color"
  name="Your Metric"
/>
```

### Custom Color Schemes

Modify the `colors` object in PerformanceChart:
```javascript
const colors = {
  accuracy: '#f59e0b',
  loss: '#ef4444',
  // Add your colors
};
```

### Loading Real Data

Replace the mock data loading:
```javascript
const loadBenchmarkData = async () => {
  setIsLoading(true);
  try {
    const response = await fetch('/api/benchmarks');
    const data = await response.json();
    setBenchmarkData(data);
  } catch (error) {
    console.error('Failed to load data:', error);
  } finally {
    setIsLoading(false);
  }
};
```

## Design Philosophy

The dashboard uses a distinctive industrial aesthetic:
- **Dark mode**: Stone and amber color palette
- **Monospace typography**: Technical, data-focused feel
- **Minimal borders**: Clean, brutalist design
- **Bold accents**: Amber highlights for key interactions
- **No rounded corners**: Sharp, professional appearance

This design avoids generic dashboard patterns and creates a memorable, professional interface suitable for ML engineering workflows.

## Performance Considerations

- Uses `useMemo` for expensive computations
- Recharts handles chart optimization
- Modal prevents unnecessary re-renders
- Efficient data filtering and sorting

## Browser Support

Tested on modern browsers:
- Chrome 100+
- Firefox 100+
- Safari 15+
- Edge 100+

## License

MIT