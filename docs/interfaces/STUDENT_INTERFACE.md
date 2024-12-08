# Interface Étudiant - M5Stack HuskyLens V2.0

## 1. Vue d'ensemble

### 1.1 Objectifs
- Interface web responsive accessible depuis tout navigateur
- Éditeur de code Quorum intégré
- Feedback en temps réel
- Visualisation des résultats
- Suivi de progression

### 1.2 Layout principal
```
+----------------------------------+
|            Header                |
|----------------------------------|
| Mission | Editor  | Results      |
| List    |        |              |
|         |        |              |
|         |        |              |
|         |        |              |
|----------------------------------|
|            Footer                |
+----------------------------------+
```

## 2. Composants d'interface

### 2.1 Header
```vue
<template>
  <header class="flex justify-between items-center p-4 bg-primary">
    <div class="flex items-center">
      <img src="logo.svg" alt="Logo" class="w-10 h-10">
      <h1 class="ml-4 text-xl font-bold">M5Stack HuskyLens Challenge</h1>
    </div>
    
    <div class="flex items-center">
      <div class="mr-4">
        <span class="font-bold">Points:</span>
        {{ userPoints }}
      </div>
      
      <div class="mr-4">
        <span class="font-bold">Mission:</span>
        {{ currentMission }}
      </div>
      
      <UserAvatar :user="currentUser" />
    </div>
  </header>
</template>
```

### 2.2 Mission List
```vue
<template>
  <div class="mission-list">
    <div class="mission-filters mb-4">
      <select v-model="filter" class="select">
        <option value="all">Toutes les missions</option>
        <option value="completed">Complétées</option>
        <option value="pending">En attente</option>
      </select>
    </div>
    
    <div class="missions">
      <MissionCard 
        v-for="mission in filteredMissions"
        :key="mission.id"
        :mission="mission"
        :active="currentMission?.id === mission.id"
        @click="selectMission(mission)"
      />
    </div>
  </div>
</template>

<style scoped>
.mission-card {
  @apply p-4 mb-2 rounded-lg cursor-pointer transition-colors;
}

.mission-card.active {
  @apply bg-primary-light border-primary;
}

.mission-card:hover {
  @apply bg-gray-100;
}
</style>
```

### 2.3 Code Editor
```vue
<template>
  <div class="code-editor-container">
    <div class="toolbar flex justify-between items-center p-2">
      <div class="actions">
        <button @click="run" :disabled="isRunning">
          <i class="fas fa-play"></i> Exécuter
        </button>
        <button @click="reset">
          <i class="fas fa-undo"></i> Réinitialiser
        </button>
      </div>
      
      <div class="settings">
        <button @click="showSettings">
          <i class="fas fa-cog"></i>
        </button>
      </div>
    </div>
    
    <MonacoEditor
      v-model="code"
      language="quorum"
      :options="editorOptions"
      @change="handleCodeChange"
    />
    
    <div class="status-bar p-2">
      <div class="flex items-center">
        <StatusIndicator :status="editorStatus" />
        <span class="ml-2">{{ statusMessage }}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
const editorOptions = {
  theme: 'vs-dark',
  fontSize: 14,
  minimap: { enabled: false },
  automaticLayout: true,
  formatOnPaste: true,
  formatOnType: true
};

const run = async () => {
  try {
    isRunning.value = true;
    const result = await submitCode(code.value);
    handleResult(result);
  } catch (error) {
    handleError(error);
  } finally {
    isRunning.value = false;
  }
};
</script>
```

### 2.4 Results Panel
```vue
<template>
  <div class="results-panel">
    <div class="tabs">
      <button 
        v-for="tab in tabs" 
        :key="tab.id"
        :class="{ active: currentTab === tab.id }"
        @click="currentTab = tab.id"
      >
        {{ tab.label }}
      </button>
    </div>
    
    <div class="tab-content">
      <!-- Console Output -->
      <div v-if="currentTab === 'console'" class="console">
        <div v-for="log in consoleOutput" :key="log.id" :class="log.type">
          {{ log.message }}
        </div>
      </div>
      
      <!-- Vision Results -->
      <div v-if="currentTab === 'vision'" class="vision-results">
        <canvas ref="visionCanvas"></canvas>
        <div class="detections">
          <DetectionItem 
            v-for="item in detections"
            :key="item.id"
            :detection="item"
          />
        </div>
      </div>
      
      <!-- Mission Status -->
      <div v-if="currentTab === 'status'" class="mission-status">
        <div class="progress">
          <div class="progress-bar" :style="{ width: progress + '%' }">
            {{ progress }}%
          </div>
        </div>
        <div class="requirements">
          <RequirementItem 
            v-for="req in requirements"
            :key="req.id"
            :requirement="req"
            :completed="isRequirementMet(req)"
          />
        </div>
      </div>
    </div>
  </div>
</template>
```

## 3. États et interactions

### 3.1 États de mission
```typescript
type MissionState = {
  id: number;
  status: 'locked' | 'available' | 'in_progress' | 'completed';
  progress: number;
  attempts: number;
  lastAttempt?: Date;
  points: number;
  requirements: {
    id: string;
    description: string;
    met: boolean;
  }[];
};

const missionStore = reactive<{
  currentMission?: MissionState;
  missions: MissionState[];
}>({
  missions: []
});
```

### 3.2 États d'exécution
```typescript
type ExecutionState = {
  status: 'idle' | 'running' | 'success' | 'error';
  message?: string;
  consoleOutput: ConsoleMessage[];
  detections: Detection[];
  performance: {
    executionTime: number;
    memoryUsage: number;
  };
};

const executionStore = reactive<ExecutionState>({
  status: 'idle',
  consoleOutput: [],
  detections: [],
  performance: {
    executionTime: 0,
    memoryUsage: 0
  }
});
```

## 4. Feedback utilisateur

### 4.1 Notifications
```typescript
const notifications = {
  success: (message: string) => {
    toast.success(message, {
      position: 'top-right',
      duration: 3000,
      dismissible: true
    });
  },
  
  error: (message: string) => {
    toast.error(message, {
      position: 'top-right',
      duration: 5000,
      dismissible: true
    });
  },
  
  missionComplete: (mission: Mission) => {
    modal.show({
      title: 'Mission Accomplie !',
      content: `
        <div class="mission-complete">
          <h3>Félicitations !</h3>
          <p>Vous avez terminé la mission "${mission.title}"</p>
          <p>Points gagnés : ${mission.points}</p>
        </div>
      `,
      buttons: [
        { label: 'Continuer', action: () => nextMission() }
      ]
    });
  }
};
```

### 4.2 Indicateurs de progression
```vue
<template>
  <div class="progress-indicators">
    <div class="mission-progress">
      <div class="progress-bar">
        <div 
          class="progress-fill"
          :style="{ width: `${progress}%` }"
          :class="{ 'animate-pulse': isRunning }"
        ></div>
      </div>
      <div class="progress-stats">
        <span>{{ completedTasks }}/{{ totalTasks }} tâches</span>
        <span>{{ points }} points</span>
      </div>
    </div>
    
    <div class="execution-status">
      <StatusIcon :status="status" />
      <span>{{ statusMessage }}</span>
      <Timer v-if="isRunning" :startTime="startTime" />
    </div>
  </div>
</template>
```

## 5. Responsive Design

### 5.1 Breakpoints
```css
/* tailwind.config.js */
module.exports = {
  theme: {
    screens: {
      'sm': '640px',
      'md': '768px',
      'lg': '1024px',
      'xl': '1280px',
    }
  }
}
```

### 5.2 Layouts responsifs
```vue
<template>
  <div class="student-interface">
    <!-- Desktop Layout -->
    <div class="hidden lg:grid grid-cols-12 gap-4">
      <div class="col-span-3">
        <MissionList />
      </div>
      <div class="col-span-6">
        <CodeEditor />
      </div>
      <div class="col-span-3">
        <ResultsPanel />
      </div>
    </div>
    
    <!-- Mobile Layout -->
    <div class="lg:hidden">
      <TabView>
        <TabPanel header="Missions">
          <MissionList />
        </TabPanel>
        <TabPanel header="Code">
          <CodeEditor />
        </TabPanel>
        <TabPanel header="Résultats">
          <ResultsPanel />
        </TabPanel>
      </TabView>
    </div>
  </div>
</template>
```

## 6. Animations et transitions

### 6.1 Transitions de vue
```css
.page-enter-active,
.page-leave-active {
  transition: opacity 0.3s, transform 0.3s;
}

.page-enter-from,
.page-leave-to {
  opacity: 0;
  transform: translateX(-10px);
}

.panel-enter-active,
.panel-leave-active {
  transition: all 0.3s ease;
}

.panel-enter-from,
.panel-leave-to {
  transform: translateY(20px);
  opacity: 0;
}
```

### 6.2 Animations d'état
```css
@keyframes pulse {
  0% { opacity: 1; }
  50% { opacity: 0.5; }
  100% { opacity: 1; }
}

.running {
  animation: pulse 1s infinite;
}

.success {
  animation: successPop 0.5s ease-out;
}

@keyframes successPop {
  0% { transform: scale(1); }
  50% { transform: scale(1.1); }
  100% { transform: scale(1); }
}
```

## 7. Accessibilité

### 7.1 Composants accessibles
```vue
<template>
  <button
    role="button"
    :aria-label="label"
    :aria-pressed="isActive"
    :tabindex="0"
    class="action-button"
    @click="handleClick"
    @keydown.enter="handleClick"
  >
    <span class="sr-only">{{ screenReaderText }}</span>
    <i :class="icon" aria-hidden="true"></i>
    {{ label }}
  </button>
</template>
```

### 7.2 Navigation au clavier
```typescript
const handleKeyboardNavigation = (event: KeyboardEvent) => {
  switch (event.key) {
    case 'Tab':
      // Gérer la navigation entre les panneaux
      break;
    case 'Escape':
      // Fermer les modales/popups
      break;
    case 'F5':
    case 'F8':
      // Raccourcis d'exécution
      event.preventDefault();
      executeCode();
      break;
  }
};
```

## 8. Support hors ligne

### 8.1 Service Worker
```typescript
// service-worker.ts
self.addEventListener('install', (event) => {
  event.waitUntil(
    caches.open('m5stack-cache-v1').then((cache) => {
      return cache.addAll([
        '/',
        '/index.html',
        '/styles.css',
        '/app.js',
        '/assets/monaco-editor/...',
        '/api/missions/...'
      ]);
    })
  );
});

self.addEventListener('fetch', (event) => {
  event.respondWith(
    caches.match(event.request).then((response) => {
      return response || fetch(event.request);
    })
  );
});
```

### 8.2 État de connexion
```vue
<template>
  <div class="connection-status">
    <div v-if="!isOnline" class="offline-banner">
      <i class="fas fa-wifi-slash"></i>
      Mode hors ligne - Synchronisation en attente
    </div>
    
    <div class="sync-queue" v-if="hasPendingSync">
      {{ pendingSyncs.length }} modifications en attente
      <button @click="syncNow" :disabled="!isOnline">
        Synchroniser maintenant
      </button>
    </div>
  </div>
</template>

<script setup>
const isOnline = ref(navigator.onLine);
const pendingSyncs = ref([]);

window.addEventListener('online', () => {
  isOnline.value = true;
  syncPendingChanges();
});

window.addEventListener('offline', () => {
  isOnline.value = false;
});
</script>
```