# Technical Documentation: Neural Network Benchmark Dashboard

## Architecture Overview

This document provides an in-depth technical explanation of the dashboard's architecture, data flow, and implementation details.

## 1. Data Loading Architecture

### Current Implementation

The dashboard uses a simulated data loading approach for development:

```javascript
const loadBenchmarkData = () => {
  setIsLoading(true);
  setTimeout(() => {
    setBenchmarkData(MOCK_DATA);
    setIsLoading(false);
  }, 500);
};
```

### Production Implementation

For production, replace with actual file fetching:

```javascript
const loadBenchmarkData = async () => {
  setIsLoading(true);
  try {
    const response = await fetch('/api/benchmark-data.json');
    if (!response.ok) throw new Error('Failed to fetch data');
    const data = await response.json();
    
    // Validate data structure
    const validatedData = data.filter(item => 
      item.experiment_id && 
      item.date && 
      item.performance
    );
    
    setBenchmarkData(validatedData);
  } catch (error) {
    console.error('Data loading error:', error);
    // Handle error state
  } finally {
    setIsLoading(false);
  }
};
```

### Data Validation

The system expects each entry to contain:
- `experiment_id`: String identifier for grouping
- `date`: ISO 8601 timestamp for sorting
- `performance`: Object with numeric metrics
- `network_config`: Configuration metadata
- `timestamps`: Start and end times

## 2. Experiment Grouping Mechanism

### Implementation

```javascript
const experimentIds = useMemo(() => {
  return [...new Set(benchmarkData.map(item => item.experiment_id))].sort();
}, [benchmarkData]);
```

### How It Works

1. **Extraction**: Maps over all benchmark entries to extract `experiment_id` values
2. **Deduplication**: Uses `Set` to remove duplicates
3. **Sorting**: Alphabetically sorts experiment IDs for consistent ordering
4. **Memoization**: Recalculates only when `benchmarkData` changes

### Experiment Filtering

```javascript
const experimentData = useMemo(() => {
  if (!selectedExperiment) return [];
  return benchmarkData
    .filter(item => item.experiment_id === selectedExperiment)
    .sort((a, b) => new Date(a.date) - new Date(b.date));
}, [benchmarkData, selectedExperiment]);
```

**Key Points**:
- Filters entries matching selected experiment
- Chronologically sorts by date (oldest to newest)
- Returns empty array if no experiment selected
- Automatically updates when data or selection changes

## 3. Chart Update Mechanism

### Reactive Updates

The charts update automatically through React's reactivity system:

```javascript
<PerformanceChart
  data={experimentData}           // Updates when experiment changes
  onDataPointClick={setSelectedEpoch}
  activeMetrics={activeMetrics}   // Updates when toggles change
/>
```

### Data Flow

```
User Action → State Change → Memo Recomputation → Component Re-render → Chart Update
```

### Example Flows

**Experiment Selection**:
```
Select exp_002 → setSelectedExperiment('exp_002') → 
experimentData memo recalculates → PerformanceChart receives new data → 
Recharts re-renders with filtered data
```

**Metric Toggle**:
```
Toggle precision → setActiveMetrics({...prev, precision: !prev.precision}) →
PerformanceChart re-renders → Line component conditionally renders
```

### Performance Optimization

The system uses `useMemo` to prevent unnecessary recalculations:

```javascript
const currentMetrics = useMemo(() => {
  if (experimentData.length === 0) return null;
  
  const latest = experimentData[experimentData.length - 1];
  const previous = experimentData.length > 1 
    ? experimentData[experimentData.length - 2] 
    : null;
  
  return {
    current: latest.performance,
    trends: previous ? {
      accuracy: latest.performance.accuracy - previous.performance.accuracy,
      loss: latest.performance.loss - previous.performance.loss,
      f1_score: latest.performance.f1_score - previous.performance.f1_score
    } : null
  };
}, [experimentData]);
```

This memo:
- Calculates only when `experimentData` changes
- Computes latest metrics and trends
- Avoids recalculation on unrelated state changes

## 4. Reload Mechanism

### Implementation

```javascript
const handleReload = () => {
  loadBenchmarkData();
};
```

### Complete Flow

1. **User clicks reload button**
   ```
   onClick → handleReload() → loadBenchmarkData()
   ```

2. **Loading state activates**
   ```
   setIsLoading(true) → Button shows "Loading..." → Button disabled
   ```

3. **Data fetched**
   ```
   setTimeout simulates fetch → MOCK_DATA loaded → setBenchmarkData(MOCK_DATA)
   ```

4. **State cascade**
   ```
   benchmarkData updates → experimentIds memo recalculates → 
   experimentData memo recalculates → All dependent components re-render
   ```

5. **Loading state cleared**
   ```
   setIsLoading(false) → Button shows "Reload Data" → Button enabled
   ```

### Data Merging Strategy

The current implementation **replaces** all data on reload. For incremental updates:

```javascript
const loadBenchmarkData = async () => {
  setIsLoading(true);
  try {
    const response = await fetch('/api/benchmark-data.json');
    const newData = await response.json();
    
    setBenchmarkData(prevData => {
      // Merge strategy: keep existing + add new entries
      const existingIds = new Set(
        prevData.map(item => `${item.experiment_id}-${item.date}`)
      );
      
      const uniqueNewData = newData.filter(
        item => !existingIds.has(`${item.experiment_id}-${item.date}`)
      );
      
      return [...prevData, ...uniqueNewData].sort(
        (a, b) => new Date(a.date) - new Date(b.date)
      );
    });
  } finally {
    setIsLoading(false);
  }
};
```

### Preventing Data Loss

The merge strategy ensures:
- **No overwrites**: Existing entries remain unchanged
- **Deduplication**: Prevents duplicate entries based on experiment ID + date
- **Chronological order**: Maintains timeline integrity
- **State preservation**: Selected experiment remains active if still valid

## 5. Data Point Interaction

### Click Handler Flow

```javascript
const CustomDot = (props) => {
  const { cx, cy, payload } = props;
  return (
    <circle
      cx={cx}
      cy={cy}
      r={6}
      onClick={() => onDataPointClick(payload)}  // Passes entire epoch data
    />
  );
};
```

### Payload Structure

The `payload` object contains the complete epoch entry:
```javascript
{
  experiment_id: "exp_001",
  date: "2025-12-16T14:35:00Z",
  network_config: { ... },
  performance: { ... },
  timestamps: { ... },
  notes: "..."
}
```

### Modal Display

```javascript
<EpochDetailModal
  epoch={selectedEpoch}           // Full epoch data
  onClose={() => setSelectedEpoch(null)}  // Clear selection
/>
```

The modal:
1. Renders when `selectedEpoch` is not null
2. Displays comprehensive metrics and configuration
3. Closes by setting `selectedEpoch` to null
4. Uses portal pattern for overlay rendering

## 6. State Management Strategy

### State Structure

```javascript
const [benchmarkData, setBenchmarkData] = useState([]);
const [selectedExperiment, setSelectedExperiment] = useState('');
const [selectedEpoch, setSelectedEpoch] = useState(null);
const [activeMetrics, setActiveMetrics] = useState({...});
const [isLoading, setIsLoading] = useState(false);
```

### State Dependencies

```
benchmarkData
  ├── experimentIds (derived)
  └── experimentData (derived, also depends on selectedExperiment)
       └── currentMetrics (derived)

selectedExperiment
  └── experimentData (derived)

activeMetrics
  └── PerformanceChart (affects rendering)

selectedEpoch
  └── EpochDetailModal (visibility and content)

isLoading
  └── Reload button (disabled state and text)
```

### Why This Structure

- **Minimal state**: Only stores what can't be derived
- **Single source of truth**: `benchmarkData` is the only data source
- **Computed values**: Use `useMemo` for expensive derivations
- **Predictable updates**: Clear data flow prevents bugs

## 7. Chart Configuration

### Recharts Setup

```javascript
<ResponsiveContainer width="100%" height={400}>
  <LineChart data={experimentData} margin={{ top: 5, right: 30, left: 20, bottom: 5 }}>
    <CartesianGrid strokeDasharray="3 3" stroke="#44403c" />
    <XAxis dataKey="date" tickFormatter={formatDate} />
    <YAxis />
    <Tooltip />
    <Legend />
    {activeMetrics.accuracy && <Line ... />}
    {activeMetrics.loss && <Line ... />}
  </LineChart>
</ResponsiveContainer>
```

### Conditional Rendering

Metrics only render when toggled on:
```javascript
{activeMetrics.accuracy && (
  <Line 
    type="monotone" 
    dataKey="performance.accuracy" 
    stroke={colors.accuracy}
  />
)}
```

This approach:
- Reduces rendering overhead for hidden metrics
- Maintains smooth performance
- Allows dynamic metric selection

### Custom Tooltip

```javascript
<Tooltip
  contentStyle={{
    backgroundColor: '#1c1917',
    border: '1px solid #44403c',
    fontFamily: 'monospace'
  }}
/>
```

Provides context on hover with styled content matching the theme.

## 8. Extension Points

### Adding New Visualizations

To add a scatter plot for loss vs accuracy:

```javascript
<ScatterChart>
  <XAxis dataKey="performance.loss" />
  <YAxis dataKey="performance.accuracy" />
  <Scatter data={experimentData} fill="#f59e0b" />
</ScatterChart>
```

### Adding Filters

To filter by date range:

```javascript
const [dateRange, setDateRange] = useState({ start: null, end: null });

const filteredData = useMemo(() => {
  return experimentData.filter(item => {
    const date = new Date(item.date);
    const inRange = (!dateRange.start || date >= dateRange.start) &&
                    (!dateRange.end || date <= dateRange.end);
    return inRange;
  });
}, [experimentData, dateRange]);
```

### Adding Export Functionality

To export data as CSV:

```javascript
const exportToCSV = () => {
  const headers = ['date', 'accuracy', 'loss', 'f1_score'];
  const rows = experimentData.map(item => [
    item.date,
    item.performance.accuracy,
    item.performance.loss,
    item.performance.f1_score
  ]);
  
  const csv = [headers, ...rows].map(row => row.join(',')).join('\n');
  const blob = new Blob([csv], { type: 'text/csv' });
  const url = URL.createObjectURL(blob);
  
  const link = document.createElement('a');
  link.href = url;
  link.download = `${selectedExperiment}-export.csv`;
  link.click();
};
```

## 9. Performance Considerations

### Memo Usage

Memoization prevents expensive recalculations:
- `experimentIds`: Only recalculates when data changes
- `experimentData`: Only recalculates when data or selection changes
- `currentMetrics`: Only recalculates when experiment data changes

### Rendering Optimization

- Conditional Line components reduce render overhead
- ResponsiveContainer handles chart resizing efficiently
- Modal prevents unnecessary parent re-renders

### Bundle Size

Current dependencies:
- React: ~45KB gzipped
- Recharts: ~95KB gzipped
- Tailwind: ~10KB (purged)
- Total: ~150KB gzipped

## 10. Testing Strategies

### Unit Testing

Test derived data computations:
```javascript
describe('experimentData', () => {
  it('filters by experiment ID', () => {
    const data = [
      { experiment_id: 'exp_001', date: '2025-12-15' },
      { experiment_id: 'exp_002', date: '2025-12-16' }
    ];
    const filtered = data.filter(item => item.experiment_id === 'exp_001');
    expect(filtered).toHaveLength(1);
  });
});
```

### Integration Testing

Test user flows:
1. Select experiment
2. Verify chart updates
3. Toggle metrics
4. Verify chart updates
5. Click data point
6. Verify modal opens

### E2E Testing

Use Playwright or Cypress to test complete workflows:
```javascript
test('benchmark dashboard workflow', async ({ page }) => {
  await page.goto('http://localhost:3000');
  await page.selectOption('select', 'exp_001');
  await page.click('button:has-text("Reload Data")');
  await expect(page.locator('.recharts-line')).toBeVisible();
});
```

## 11. Deployment Considerations

### Environment Variables

For production API endpoints:
```javascript
const API_URL = import.meta.env.VITE_API_URL || '/api';

const loadBenchmarkData = async () => {
  const response = await fetch(`${API_URL}/benchmark-data.json`);
  // ...
};
```

### Build Optimization

Vite automatically:
- Minifies JavaScript
- Tree-shakes unused code
- Optimizes CSS with Tailwind purge
- Generates efficient chunks

### Hosting

Static hosting requirements:
- Serve built files from `dist/` directory
- Configure routing for SPA (if using React Router)
- Set appropriate cache headers
- Enable gzip/brotli compression

## Conclusion

This dashboard implements a robust, scalable architecture for neural network benchmark visualization. The reactive design ensures automatic updates, while memoization maintains performance. The modular component structure allows easy extension and customization for specific ML workflows.