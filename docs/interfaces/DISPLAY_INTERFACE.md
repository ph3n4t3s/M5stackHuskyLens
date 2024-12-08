# Interface de Visualisation (Beamer) - M5Stack HuskyLens V2.0

## 1. Vue d'ensemble

### 1.1 Objectifs
- Affichage en temps réel de l'état du jeu
- Visualisation des classements
- Affichage des missions actives
- Feedback visuel des événements
- Vue du M5Stack et HuskyLens

### 1.2 Layout principal
```
+----------------------------------------+
|                Header                   |
|----------------------------------------|
| Mission   |  Classement  |  Activité   |
| Active    |  Général     |  Temps réel |
|          |              |             |
|----------------------------------------|
|            Visualisation               |
|            M5Stack/HuskyLens          |
+----------------------------------------+
```

## 2. Composants d'affichage

### 2.1 En-tête dynamique
```vue
<template>
  <header class="beamer-header">
    <div class="game-info">
      <h1>{{ gameTitle }}</h1>
      <div class="mission-counter">
        Mission {{ currentMission }} / {{ totalMissions }}
      </div>
    </div>
    
    <div class="time-display">
      <div class="current-time">{{ currentTime }}</div>
      <div class="mission-time">
        Temps restant: {{ remainingTime }}
      </div>
    </div>
    
    <div class="status-indicators">
      <StatusIndicator 
        v-for="indicator in indicators"
        :key="indicator.id"
        :status="indicator.status"
      />
    </div>
  </header>
</template>
```

### 2.2 Panneau de mission
```vue
<template>
  <div class="mission-panel">
    <div class="mission-header">
      <h2>Mission en cours</h2>
      <div class="mission-stats">
        <div class="completion-rate">
          {{ completionRate }}% complété
        </div>
        <div class="active-users">
          {{ activeUsers }} participants actifs
        </div>
      </div>
    </div>
    
    <div class="mission-content">
      <div class="mission-description">
        {{ currentMission.description }}
      </div>
      
      <div class="mission-requirements">
        <RequirementList 
          :requirements="currentMission.requirements"
          animated
        />
      </div>
    </div>
    
    <div class="mission-progress">
      <ProgressBar 
        :value="missionProgress"
        animated
      />
    </div>
  </div>
</template>
```

### 2.3 Classement
```vue
<template>
  <div class="leaderboard">
    <h2>Classement</h2>
    
    <div class="top-players">
      <div class="podium">
        <PodiumPosition
          v-for="position in topThree"
          :key="position.rank"
          :player="position"
          animated
        />
      </div>
    </div>
    
    <div class="player-list">
      <TransitionGroup name="list">
        <PlayerRow
          v-for="player in players"
          :key="player.id"
          :player="player"
          :highlighted="player.isActive"
        />
      </TransitionGroup>
    </div>
    
    <div class="score-statistics">
      <StatisticCard
        v-for="stat in statistics"
        :key="stat.id"
        :label="stat.label"
        :value="stat.value"
      />
    </div>
  </div>
</template>
```

### 2.4 Activité en temps réel
```vue
<template>
  <div class="activity-feed">
    <h2>Activité en direct</h2>
    
    <div class="events-container">
      <TransitionGroup name="fade">
        <EventCard
          v-for="event in recentEvents"
          :key="event.id"
          :event="event"
          :type="event.type"
        />
      </TransitionGroup>
    </div>
    
    <div class="activity-metrics">
      <MetricDisplay
        v-for="metric in metrics"
        :key="metric.id"
        :metric="metric"
        animated
      />
    </div>
  </div>
</template>
```

## 3. Visualisation M5Stack

### 3.1 Vue caméra
```vue
<template>
  <div class="camera-view">
    <div class="camera-feed">
      <video ref="videoFeed" :src="cameraStream"></video>
      
      <div class="detection-overlay">
        <DetectionBox
          v-for="detection in detections"
          :key="detection.id"
          :box="detection.bounds"
          :label="detection.label"
          animated
        />
      </div>
    </div>
    
    <div class="camera-controls">
      <button @click="toggleMode">
        Mode: {{ currentMode }}
      </button>
      <button @click="captureSnapshot">
        Capture
      </button>
    </div>
  </div>
</template>
```

### 3.2 Informations M5Stack
```vue
<template>
  <div class="m5stack-info">
    <div class="hardware-status">
      <StatusIndicator
        :status="connectionStatus"
        label="Connexion"
      />
      <StatusIndicator
        :status="huskyLensStatus"
        label="HuskyLens"
      />
    </div>
    
    <div class="sensor-data">
      <DataCard
        v-for="sensor in sensorData"
        :key="sensor.id"
        :label="sensor.label"
        :value="sensor.value"
        :unit="sensor.unit"
      />
    </div>
    
    <div class="system-metrics">
      <MetricGauge
        v-for="metric in systemMetrics"
        :key="metric.id"
        :value="metric.value"
        :max="metric.max"
        :label="metric.label"
      />
    </div>
  </div>
</template>
```

## 4. Animations et transitions

### 4.1 Transitions globales
```css
/* Transition pour les changements de mission */
.mission-transition-enter-active,
.mission-transition-leave-active {
  transition: all 0.5s ease;
}

.mission-transition-enter-from,
.mission-transition-leave-to {
  opacity: 0;
  transform: translateY(30px);
}

/* Transition pour le classement */
.leaderboard-transition-enter-active,
.leaderboard-transition-leave-active {
  transition: all 0.3s ease;
}

.leaderboard-transition-enter-from,
.leaderboard-transition-leave-to {
  transform: scale(0.95);
  opacity: 0;
}
```

### 4.2 Animations d'événements
```typescript
const animateEvent = (event: GameEvent) => {
  const animation = computed(() => {
    switch (event.type) {
      case 'success':
        return 'bounce-success';
      case 'achievement':
        return 'pop-achievement';
      case 'update':
        return 'slide-update';
      default:
        return 'fade';
    }
  });
  
  return {
    enter: (el: HTMLElement) => {
      gsap.from(el, {
        duration: 0.5,
        opacity: 0,
        y: 30,
        scale: 0.95,
        ease: 'back.out'
      });
    },
    leave: (el: HTMLElement) => {
      gsap.to(el, {
        duration: 0.3,
        opacity: 0,
        y: -30,
        scale: 0.95
      });
    }
  };
};
```

## 5. Modes d'affichage

### 5.1 Mode normal
```vue
<template>
  <div class="display-normal">
    <div class="grid-layout">
      <MissionPanel class="mission-area" />
      <LeaderboardPanel class="leaderboard-area" />
      <ActivityPanel class="activity-area" />
      <CameraView class="camera-area" />
    </div>
  </div>
</template>
```

### 5.2 Mode focus mission
```vue
<template>
  <div class="display-focus-mission">
    <div class="mission-focus-layout">
      <div class="mission-details">
        <h2>{{ currentMission.title }}</h2>
        <div class="mission-content">
          {{ currentMission.description }}
        </div>
      </div>
      
      <div class="side-panel">
        <MiniLeaderboard />
        <ActivitySummary />
      </div>
    </div>
  </div>
</template>
```

### 5.3 Mode célébration
```vue
<template>
  <div class="display-celebration">
    <div class="celebration-content">
      <h1>Mission Accomplie !</h1>
      
      <div class="winners-podium">
        <PodiumDisplay
          :winners="topPlayers"
          animated
        />
      </div>
      
      <div class="mission-stats">
        <StatisticDisplay
          v-for="stat in missionStats"
          :key="stat.id"
          :statistic="stat"
        />
      </div>
    </div>
    
    <FireworksEffect />
  </div>
</template>
```

## 6. Gestion des événements

### 6.1 Gestionnaire d'événements
```typescript
class DisplayEventManager {
  private events: Queue<DisplayEvent> = new Queue();
  private isProcessing: boolean = false;
  
  public async addEvent(event: DisplayEvent) {
    this.events.enqueue(event);
    if (!this.isProcessing) {
      await this.processEvents();
    }
  }
  
  private async processEvents() {
    this.isProcessing = true;
    
    while (!this.events.isEmpty()) {
      const event = this.events.dequeue();
      await this.displayEvent(event);
    }
    
    this.isProcessing = false;
  }
  
  private async displayEvent(event: DisplayEvent) {
    switch (event.type) {
      case 'achievement':
        await this.showAchievement(event);
        break;
      case 'mission_complete':
        await this.showMissionComplete(event);
        break;
      case 'leaderboard_update':
        await this.updateLeaderboard(event);
        break;
    }
  }
}
```

### 6.2 Notifications visuelles
```vue
<template>
  <div class="notification-manager">
    <TransitionGroup name="notification">
      <NotificationCard
        v-for="notification in activeNotifications"
        :key="notification.id"
        :notification="notification"
        @complete="removeNotification"
      >
        <template #icon>
          <NotificationIcon :type="notification.type" />
        </template>
        
        <template #content>
          <div class="notification-content">
            <h3>{{ notification.title }}</h3>
            <p>{{ notification.message }}</p>
          </div>
        </template>
      </NotificationCard>
    </TransitionGroup>
  </div>
</template>
```

## 7. Performance

### 7.1 Optimisation du rendu
```typescript
// Utilisation de la virtualisation pour les listes longues
const useVirtualList = (items: Ref<any[]>, options = {}) => {
  const containerRef = ref<HTMLElement | null>(null);
  const itemSize = options.itemSize || 50;
  
  const visibleItems = computed(() => {
    if (!containerRef.value) return [];
    
    const container = containerRef.value;
    const scrollTop = container.scrollTop;
    const containerHeight = container.clientHeight;
    
    const startIndex = Math.floor(scrollTop / itemSize);
    const endIndex = Math.ceil((scrollTop + containerHeight) / itemSize);
    
    return items.value.slice(startIndex, endIndex + 1);
  });
  
  return {
    containerRef,
    visibleItems,
    containerStyle: {
      height: `${items.value.length * itemSize}px`,
      position: 'relative'
    }
  };
};
```

### 7.2 Gestion de la mémoire
```typescript
// Nettoyage automatique des événements anciens
const useEventCleanup = (events: Ref<DisplayEvent[]>) => {
  const MAX_EVENTS = 100;
  const CLEANUP_INTERVAL = 60000; // 1 minute
  
  onMounted(() => {
    const interval = setInterval(() => {
      if (events.value.length > MAX_EVENTS) {
        events.value = events.value.slice(-MAX_EVENTS);
      }
    }, CLEANUP_INTERVAL);
    
    onUnmounted(() => {
      clearInterval(interval);
    });
  });
};
```

## 8. Configuration

### 8.1 Options d'affichage
```typescript
interface DisplayConfig {
  layout: {
    mode: 'normal' | 'focus' | 'celebration';
    gridColumns: number;
    aspectRatio: string;
  };
  
  updates: {
    frequency: number;
    smooth: boolean;
    transitionDuration: number;
  };
  
  camera: {
    fps: number;
    quality: 'high' | 'medium' | 'low';
    showOverlay: boolean;
  };
  
  notifications: {
    duration: number;
    maxVisible: number;
    position: 'top' | 'bottom';
  };
}
```

### 8.2 Thèmes
```typescript
const themes = {
  light: {
    colors: {
      background: '#ffffff',
      primary: '#007bff',
      secondary: '#6c757d',
      success: '#28a745',
      warning: '#ffc107',
      error: '#dc3545',
      text: '#212529'
    },
    
    fonts: {
      primary: 'Roboto, sans-serif',
      secondary: 'Open Sans, sans-serif',
      monospace: 'Fira Code, monospace'
    },
    
    spacing: {
      small: '0.5rem',
      medium: '1rem',
      large: '1.5rem'
    },
    
    animations: {
      duration: '0.3s',
      timing: 'ease-in-out'
    }
  },
  
  dark: {
    colors: {
      background: '#1a1a1a',
      primary: '#3498db',
      secondary: '#95a5a6',
      success: '#2ecc71',
      warning: '#f1c40f',
      error: '#e74c3c',
      text: '#ecf0f1'
    }
  }
};
```