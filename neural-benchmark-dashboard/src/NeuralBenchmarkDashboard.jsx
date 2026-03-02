import React, { useState, useEffect, useMemo } from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer } from 'recharts';

const ExperimentSelector = ({ experiments, selectedExperiment, onSelect }) => {
  return (
    <div className="relative">
      <label className="block text-xs font-medium tracking-wider uppercase text-stone-500 mb-2">
        Experiment
      </label>
      <select
        value={selectedExperiment}
        onChange={(e) => onSelect(e.target.value)}
        className="w-full bg-stone-900 border-2 border-stone-700 rounded-none px-4 py-3 text-stone-100 
                   font-mono text-sm focus:outline-none focus:border-amber-500 transition-colors
                   appearance-none cursor-pointer hover:border-stone-600"
      >
        <option value="">Select an experiment</option>
        {experiments.map((exp) => (
          <option key={exp} value={exp} className="bg-stone-900">
            {exp}
          </option>
        ))}
      </select>
      <div className="pointer-events-none absolute right-4 top-[42px] text-amber-500">
        <svg width="12" height="8" viewBox="0 0 12 8" fill="currentColor">
          <path d="M1 1l5 5 5-5" stroke="currentColor" strokeWidth="2" fill="none"/>
        </svg>
      </div>
    </div>
  );
};

const MetricCard = ({ label, value, format = 'percent', trend }) => {
  const formattedValue = format === 'percent' 
    ? `${(value * 100).toFixed(1)}%` 
    : format === 'decimal'
    ? value.toFixed(3)
    : value;

  return (
    <div className="bg-stone-900 border-l-4 border-amber-500 p-5">
      <div className="text-xs font-medium tracking-wider uppercase text-stone-500 mb-2">
        {label}
      </div>
      <div className="flex items-baseline justify-between">
        <div className="text-3xl font-bold text-stone-100 font-mono">
          {formattedValue}
        </div>
        {trend !== undefined && trend !== null && (
          <div className={`text-sm font-mono ${trend > 0 ? 'text-emerald-400' : 'text-red-400'}`}>
            {trend > 0 ? '+' : ''}{(trend * 100).toFixed(1)}%
          </div>
        )}
      </div>
    </div>
  );
};

const PerformanceChart = ({ data, onDataPointClick, activeMetrics }) => {
  const colors = {
    accuracy: '#f59e0b',
    loss: '#ef4444',
    precision: '#10b981',
    recall: '#3b82f6',
    f1_score: '#8b5cf6'
  };

  const formatDate = (dateString) => {
    const date = new Date(dateString);
    return date.toLocaleTimeString('en-US', { hour: '2-digit', minute: '2-digit' });
  };

  const CustomDot = (props) => {
    const { cx, cy, payload, dataKey } = props;
    return (
      <circle
        cx={cx}
        cy={cy}
        r={6}
        fill={colors[dataKey?.split('.')[1]] || colors.accuracy}
        stroke="#1c1917"
        strokeWidth={2}
        className="cursor-pointer hover:r-8 transition-all"
        onClick={() => onDataPointClick(payload)}
      />
    );
  };

  return (
    <div className="bg-stone-900 p-6 border-2 border-stone-800">
      <div className="flex items-center justify-between mb-6">
        <h3 className="text-xs font-medium tracking-wider uppercase text-stone-500">
          Performance Evolution
        </h3>
      </div>
      <ResponsiveContainer width="100%" height={400}>
        <LineChart data={data} margin={{ top: 5, right: 30, left: 20, bottom: 5 }}>
          <CartesianGrid strokeDasharray="3 3" stroke="#44403c" />
          <XAxis 
            dataKey="date" 
            tickFormatter={formatDate}
            stroke="#78716c"
            style={{ fontSize: '12px', fontFamily: 'monospace' }}
          />
          <YAxis stroke="#78716c" style={{ fontSize: '12px', fontFamily: 'monospace' }} />
          <Tooltip
            contentStyle={{ backgroundColor: '#1c1917', border: '1px solid #44403c', borderRadius: '0', fontFamily: 'monospace', fontSize: '12px' }}
            labelStyle={{ color: '#a8a29e' }}
          />
          <Legend wrapperStyle={{ fontFamily: 'monospace', fontSize: '12px', paddingTop: '20px' }} />
          {Object.entries(activeMetrics).map(([key, isActive]) => (
            isActive && (
              <Line 
                key={key}
                type="monotone" 
                dataKey={`performance.${key}`} 
                stroke={colors[key]}
                strokeWidth={key === 'accuracy' || key === 'loss' ? 3 : 2}
                name={key.charAt(0).toUpperCase() + key.slice(1).replace('_', ' ')}
                dot={<CustomDot />}
              />
            )
          ))}
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
};

const MetricToggle = ({ metrics, activeMetrics, onToggle }) => {
  return (
    <div className="bg-stone-900 p-5 border-2 border-stone-800">
      <h3 className="text-xs font-medium tracking-wider uppercase text-stone-500 mb-4">
        Visible Metrics
      </h3>
      <div className="space-y-3">
        {Object.keys(metrics).map((metric) => (
          <label key={metric} className="flex items-center justify-between cursor-pointer group">
            <span className="text-sm text-stone-300 font-mono capitalize group-hover:text-stone-100 transition-colors">
              {metric.replace('_', ' ')}
            </span>
            <button
              onClick={() => onToggle(metric)}
              className={`w-12 h-6 rounded-full transition-all duration-300 ${activeMetrics[metric] ? 'bg-amber-500' : 'bg-stone-700'}`}
            >
              <div className={`w-5 h-5 bg-stone-100 rounded-full transform transition-transform duration-300 ${activeMetrics[metric] ? 'translate-x-6' : 'translate-x-1'}`} />
            </button>
          </label>
        ))}
      </div>
    </div>
  );
};

const HyperparameterDetailModal = ({ epoch, onClose }) => {
  if (!epoch) return null;
  const hiddenLayers = epoch.network_config.layers.filter(l => l.type === 'hidden');
  const outputLayer = epoch.network_config.layers.find(l => l.type === 'output');

  return (
    <div className="fixed inset-0 bg-black bg-opacity-80 flex items-center justify-center z-50 p-4" onClick={onClose}>
      <div className="bg-stone-900 border-4 border-amber-500 max-w-5xl w-full max-h-[90vh] overflow-y-auto" onClick={(e) => e.stopPropagation()}>
        <div className="sticky top-0 bg-stone-900 border-b-4 border-amber-500 p-6 flex justify-between items-center z-10">
          <div>
            <h2 className="text-2xl font-bold text-amber-400 font-mono mb-1">Model Configuration Inspector</h2>
            <div className="text-sm text-stone-400 font-mono">{epoch.experiment_id} | {new Date(epoch.date).toLocaleString()}</div>
          </div>
          <button onClick={onClose} className="text-stone-400 hover:text-amber-400 text-3xl font-bold px-3">×</button>
        </div>
        <div className="p-6 space-y-6">
          <div className="bg-stone-950 border-2 border-amber-500 p-6">
            <div className="grid grid-cols-2 gap-6">
              <div className="bg-stone-900 border-l-4 border-amber-500 p-4">
                <div className="text-xs text-stone-500 uppercase mb-1">Learning Rate</div>
                <div className="text-3xl font-mono font-bold text-amber-400">{epoch.network_config.learning_rate}</div>
              </div>
              <div className="bg-stone-900 border-l-4 border-amber-500 p-4">
                <div className="text-xs text-stone-500 uppercase mb-1">Batch Size</div>
                <div className="text-3xl font-mono font-bold text-amber-400">{epoch.network_config.batch_size}</div>
              </div>
            </div>
          </div>
          <div className="bg-stone-950 border-2 border-stone-700 p-6">
            <div className="space-y-3">
              {hiddenLayers.map((layer, idx) => (
                <div key={idx} className="bg-stone-900 border-l-4 border-blue-500 p-4 flex justify-between">
                  <div className="font-mono text-blue-400">Layer {idx + 1} | {layer.neuron_type}</div>
                  <div className="text-stone-100 font-mono font-bold">{layer.size} units</div>
                </div>
              ))}
              {outputLayer && (
                <div className="bg-stone-900 border-l-4 border-green-500 p-4 flex justify-between">
                  <div className="font-mono text-green-400">Output | {outputLayer.neuron_type}</div>
                  <div className="text-stone-100 font-mono font-bold">{outputLayer.size} units</div>
                </div>
              )}
            </div>
          </div>
        </div>
        <div className="sticky bottom-0 bg-stone-900 border-t-2 border-stone-800 p-4">
          <button onClick={onClose} className="w-full bg-amber-500 text-stone-950 font-bold py-3 font-mono uppercase text-sm">Close</button>
        </div>
      </div>
    </div>
  );
};

const NeuralBenchmarkDashboard = () => {
  const [benchmarkData, setBenchmarkData] = useState([]);
  const [selectedExperiment, setSelectedExperiment] = useState('');
  const [selectedEpoch, setSelectedEpoch] = useState(null);
  const [isLoading, setIsLoading] = useState(false);
  const [activeMetrics, setActiveMetrics] = useState({
    accuracy: true, loss: true, precision: false, recall: false, f1_score: false
  });

  const loadBenchmarkData = async () => {
    setIsLoading(true);
    try {
      const response = await fetch('/benchmark-data.json');
      const data = await response.json();
      setBenchmarkData(data);
    } catch (error) {
      console.error("Failed to load benchmark data:", error);
    } finally {
      setIsLoading(false);
    }
  };

  useEffect(() => {
    loadBenchmarkData();
  }, []);

  const experimentIds = useMemo(() => {
    return [...new Set(benchmarkData.map(item => item.experiment_id))].sort();
  }, [benchmarkData]);

  const experimentData = useMemo(() => {
    if (!selectedExperiment) return [];
    return benchmarkData
      .filter(item => item.experiment_id === selectedExperiment)
      .sort((a, b) => new Date(a.date) - new Date(b.date));
  }, [benchmarkData, selectedExperiment]);

  const currentMetrics = useMemo(() => {
    if (experimentData.length === 0) return null;
    const latest = experimentData[experimentData.length - 1];
    const previous = experimentData.length > 1 ? experimentData[experimentData.length - 2] : null;
    return {
      current: latest.performance,
      trends: previous ? {
        accuracy: latest.performance.accuracy - previous.performance.accuracy,
        loss: latest.performance.loss - previous.performance.loss,
        f1_score: latest.performance.f1_score - previous.performance.f1_score
      } : null
    };
  }, [experimentData]);

  return (
    <div className="min-h-screen bg-stone-950 text-stone-100">
      <div className="max-w-[1800px] mx-auto p-8">
        <header className="mb-12">
          <div className="border-l-8 border-amber-500 pl-6 mb-8">
            <h1 className="text-5xl font-black tracking-tighter mb-2">Neural Network Benchmark</h1>
            <p className="text-stone-400 text-lg font-mono">Performance tracking and hyperparameter analysis</p>
          </div>
          <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
            <div className="lg:col-span-2">
              <ExperimentSelector experiments={experimentIds} selectedExperiment={selectedExperiment} onSelect={setSelectedExperiment} />
            </div>
            <button onClick={loadBenchmarkData} disabled={isLoading} className="bg-amber-500 hover:bg-amber-600 disabled:bg-stone-700 text-stone-950 font-bold py-3 font-mono uppercase text-sm">
              {isLoading ? 'Loading...' : 'Reload Data'}
            </button>
          </div>
        </header>

        {selectedExperiment && experimentData.length > 0 ? (
          <div className="space-y-6">
            <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
              <MetricCard label="Accuracy" value={currentMetrics.current.accuracy} trend={currentMetrics.trends?.accuracy} />
              <MetricCard label="Loss" value={currentMetrics.current.loss} format="decimal" trend={currentMetrics.trends?.loss} />
              <MetricCard label="F1 Score" value={currentMetrics.current.f1_score} trend={currentMetrics.trends?.f1_score} />
            </div>
            <div className="grid grid-cols-1 lg:grid-cols-4 gap-6">
              <div className="lg:col-span-3">
                <PerformanceChart data={experimentData} onDataPointClick={setSelectedEpoch} activeMetrics={activeMetrics} />
              </div>
              <MetricToggle metrics={activeMetrics} activeMetrics={activeMetrics} onToggle={(m) => setActiveMetrics(p => ({...p, [m]: !p[m]}))} />
            </div>
            <div className="bg-stone-900 border-2 border-stone-800 p-6">
              <div className="grid grid-cols-2 md:grid-cols-4 gap-6">
                <div><div className="text-xs text-stone-400 mb-1">Total Epochs</div><div className="text-2xl font-mono">{experimentData.length}</div></div>
                <div><div className="text-xs text-stone-400 mb-1">Data Size</div><div className="text-2xl font-mono">{experimentData[0].training_data_size.toLocaleString()}</div></div>
                <div><div className="text-xs text-stone-400 mb-1">Avg Duration</div><div className="text-2xl font-mono">{(experimentData.reduce((s, d) => s + d.performance.training_duration_sec, 0) / experimentData.length).toFixed(1)}s</div></div>
                <div><div className="text-xs text-stone-400 mb-1">Best Acc</div><div className="text-2xl font-mono text-amber-400">{(Math.max(...experimentData.map(d => d.performance.accuracy)) * 100).toFixed(1)}%</div></div>
              </div>
            </div>
          </div>
        ) : (
          <div className="bg-stone-900 border-2 border-stone-800 p-20 text-center text-stone-500 font-mono">
            {selectedExperiment ? 'No data available' : 'Select an experiment to begin'}
          </div>
        )}
      </div>
      {selectedEpoch && <HyperparameterDetailModal epoch={selectedEpoch} onClose={() => setSelectedEpoch(null)} />}
    </div>
  );
};

export default NeuralBenchmarkDashboard;