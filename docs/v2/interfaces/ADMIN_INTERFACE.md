# Interface Administrateur - M5Stack HuskyLens V2.0

## 1. Vue d'ensemble

### 1.1 Objectifs
- Gestion des missions et des participants
- Monitoring en temps réel
- Configuration du système
- Visualisation des statistiques
- Contrôle du M5Stack

### 1.2 Layout principal
```
+----------------------------------------+
|                Header                   |
|----------------------------------------|
| Sidebar |  Main Content    | Live Info  |
|         |                 |            |
|         |                 |            |
|         |                 |            |
|----------------------------------------|
|              Status Bar                 |
+----------------------------------------+
```

## 2. Tableaux de bord

### 2.1 Dashboard principal
```vue
<template>
  <div class="admin-dashboard">
    <div class="grid grid-cols-3 gap-4">
      <!-- État système -->
      <SystemStatusCard>
        <template #header>État Système</template>
        <template #content>
          <SystemMetrics :metrics="systemMetrics" />
          <ConnectionStatus :status="connectionStatus" />
        </template>
      </SystemStatusCard>

      <!-- Statistiques participants -->
      <ParticipantStatsCard>
        <template #header>Participants</template>
        <template #content>
          <div class="stats">
            <div class="stat">
              <span class="value">{{ activeUsers }}</span>
              <span class="label">Actifs</span>
            </div>
            <div class="stat">
              <span class="value">{{ submissionsCount }}</span>
              <span class="label">Soumissions</span>
            </div>
          </div>
        </template>
      </ParticipantStatsCard>

      <!-- État missions -->
      <MissionProgressCard>
        <template #header>Progrès Missions</template>
        <template #content>
          <MissionProgress :missions="missions" />
        </template>
      </MissionProgressCard>
    </div>
  </div>
</template>
```

### 2.2 Monitoring en temps réel
```vue
<template>
  <div class="real-time-monitoring">
    <!-- Graphiques en temps réel -->
    <div class="charts-container">
      <LineChart 
        :data="submissionsData"
        :options="chartOptions"
      />
      <BarChart 
        :data="participationData"
        :options="chartOptions"
      />
    </div>

    <!-- Logs système -->
    <div class="system-logs">
      <LogViewer 
        :logs="systemLogs"
        :filter="logFilter"
        @filter-change="updateLogFilter"
      />
    </div>

    <!-- Alertes -->
    <div class="alerts-panel">
      <AlertsList 
        :alerts="activeAlerts"
        @acknowledge="acknowledgeAlert"
      />
    </div>
  </div>
</template>
```

## 3. Gestion des missions

### 3.1 Configuration des missions
```vue
<template>
  <div class="mission-manager">
    <div class="toolbar">
      <button @click="createMission">
        <i class="fas fa-plus"></i> Nouvelle Mission
      </button>
      <button @click="importMissions">
        <i class="fas fa-file-import"></i> Importer
      </button>
    </div>

    <div class="missions-list">
      <DraggableList v-model="missions">
        <template #item="{ mission }">
          <MissionCard
            :mission="mission"
            :active="mission.active"
            @edit="editMission(mission)"
            @toggle="toggleMission(mission)"
            @delete="deleteMission(mission)"
          >
            <template #content>
              <div class="mission-info">
                <h3>{{ mission.title }}</h3>
                <p>{{ mission.description }}</p>
                <div class="mission-stats">
                  <span>{{ mission.completions }} complétions</span>
                  <span>{{ mission.averageTime }} temps moyen</span>
                </div>
              </div>
            </template>
          </MissionCard>
        </template>
      </DraggableList>
    </div>

    <!-- Éditeur de mission -->
    <MissionEditor
      v-if="showEditor"
      :mission="selectedMission"
      @save="saveMission"
      @cancel="closeEditor"
    />
  </div>
</template>
```

### 3.2 Validation des résultats
```vue
<template>
  <div class="results-validator">
    <div class="validation-rules">
      <h3>Règles de validation</h3>
      <ValidationRuleEditor
        v-model="validationRules"
        @update="updateRules"
      />
    </div>

    <div class="test-cases">
      <h3>Cas de test</h3>
      <TestCaseManager
        :testCases="testCases"
        @add="addTestCase"
        @remove="removeTestCase"
      />
    </div>

    <div class="automatic-validation">
      <h3>Validation automatique</h3>
      <ValidationSettings
        :settings="validationSettings"
        @update="updateSettings"
      />
    </div>
  </div>
</template>
```

## 4. Gestion des participants

### 4.1 Liste des participants
```vue
<template>
  <div class="participants-manager">
    <div class="filters">
      <SearchInput v-model="search" />
      <StatusFilter v-model="statusFilter" />
      <SortSelector v-model="sortBy" />
    </div>

    <div class="participants-table">
      <DataTable
        :data="filteredParticipants"
        :columns="columns"
        :sorting="sorting"
        @sort="updateSort"
      >
        <template #cell-status="{ participant }">
          <StatusBadge :status="participant.status" />
        </template>

        <template #cell-actions="{ participant }">
          <button @click="viewDetails(participant)">
            <i class="fas fa-eye"></i>
          </button>
          <button @click="resetProgress(participant)">
            <i class="fas fa-redo"></i>
          </button>
        </template>
      </DataTable>
    </div>

    <!-- Détails participant -->
    <ParticipantDetails
      v-if="selectedParticipant"
      :participant="selectedParticipant"
      @close="closeDetails"
    />
  </div>
</template>
```

### 4.2 Suivi des progrès
```vue
<template>
  <div class="progress-tracker">
    <div class="progress-overview">
      <div class="progress-charts">
        <CompletionChart :data="completionData" />
        <TimeChart :data="timeData" />
      </div>

      <div class="progress-stats">
        <StatCard
          v-for="stat in stats"
          :key="stat.id"
          :label="stat.label"
          :value="stat.value"
          :trend="stat.trend"
        />
      </div>
    </div>

    <div class="individual-progress">
      <ProgressTable
        :participants="participants"
        :missions="missions"
        @cell-click="showSubmissionDetails"
      />
    </div>
  </div>
</template>
```

## 5. Configuration système

### 5.1 Paramètres système
```vue
<template>
  <div class="system-settings">
    <form @submit.prevent="saveSettings">
      <div class="setting-group">
        <h3>Configuration M5Stack</h3>
        <FormField
          v-model="settings.m5stack.port"
          label="Port USB"
          type="text"
        />
        <FormField
          v-model="settings.m5stack.baudRate"
          label="Baud Rate"
          type="number"
        />
      </div>

      <div class="setting-group">
        <h3>Configuration MQTT</h3>
        <FormField
          v-model="settings.mqtt.broker"
          label="Broker URL"
          type="text"
        />
        <FormField
          v-model="settings.mqtt.port"
          label="Port"
          type="number"
        />
      </div>

      <div class="setting-group">
        <h3>Paramètres missions</h3>
        <FormField
          v-model="settings.missions.timeLimit"
          label="Temps limite (s)"
          type="number"
        />
        <FormField
          v-model="settings.missions.maxAttempts"
          label="Tentatives max"
          type="number"
        />
      </div>

      <button type="submit">Sauvegarder</button>
    </form>
  </div>
</template>
```

### 5.2 Gestion des backups
```vue
<template>
  <div class="backup-manager">
    <div class="backup-status">
      <h3>État des sauvegardes</h3>
      <div class="status-info">
        <div class="last-backup">
          Dernière sauvegarde: {{ lastBackup }}
        </div>
        <div class="next-backup">
          Prochaine sauvegarde: {{ nextBackup }}
        </div>
      </div>
    </div>

    <div class="backup-actions">
      <button @click="createBackup">
        Créer sauvegarde
      </button>
      <button @click="scheduleBackup">
        Planifier sauvegarde
      </button>
    </div>

    <div class="backup-list">
      <DataTable
        :data="backups"
        :columns="backupColumns"
      >
        <template #actions="{ backup }">
          <button @click="restore(backup)">
            Restaurer
          </button>
          <button @click="download(backup)">
            Télécharger
          </button>
          <button @click="delete(backup)">
            Supprimer
          </button>
        </template>
      </DataTable>
    </div>
  </div>
</template>
```

## 6. Gestion des erreurs

### 6.1 Journal des erreurs
```vue
<template>
  <div class="error-log">
    <div class="filters">
      <DateRangePicker v-model="dateRange" />
      <SeverityFilter v-model="severity" />
      <TypeFilter v-model="errorType" />
    </div>

    <div class="log-viewer">
      <LogTable
        :logs="filteredLogs"
        :columns="logColumns"
        @row-click="showDetails"
      >
        <template #severity="{ log }">
          <SeverityBadge :level="log.severity" />
        </template>
      </LogTable>
    </div>

    <div class="error-details" v-if="selectedError">
      <ErrorDetails
        :error="selectedError"
        @close="closeDetails"
      />
    </div>
  </div>
</template>
```

### 6.2 Gestion des alertes
```vue
<template>
  <div class="alert-manager">
    <div class="active-alerts">
      <h3>Alertes actives</h3>
      <AlertList
        :alerts="activeAlerts"
        @acknowledge="acknowledgeAlert"
        @resolve="resolveAlert"
      />
    </div>

    <div class="alert-rules">
      <h3>Règles d'alerte</h3>
      <AlertRuleEditor
        :rules="alertRules"
        @add="addRule"
        @update="updateRule"
        @delete="deleteRule"
      />
    </div>

    <div class="alert-history">
      <h3>Historique</h3>
      <AlertHistory
        :alerts="alertHistory"
        :filters="historyFilters"
      />
    </div>
  </div>
</template>
```

## 7. Analytics

### 7.1 Tableau de bord analytique
```vue
<template>
  <div class="analytics-dashboard">
    <div class="metrics-overview">
      <MetricCard
        v-for="metric in metrics"
        :key="metric.id"
        :metric="metric"
      />
    </div>

    <div class="charts">
      <div class="chart-container">
        <h3>Taux de complétion</h3>
        <CompletionRateChart :data="completionData" />
      </div>

      <div class="chart-container">
        <h3>Temps moyen par mission</h3>
        <TimePerMissionChart :data="timeData" />
      </div>

      <div class="chart-container">
        <h3>Distribution des scores</h3>
        <ScoreDistributionChart :data="scoreData" />
      </div>
    </div>

    <div class="detailed-stats">
      <StatisticsTable
        :stats="detailedStats"
        :columns="statsColumns"
      />
    </div>
  </div>
</template>
```

### 7.2 Rapports
```vue
<template>
  <div class="reports-manager">
    <div class="report-generator">
      <h3>Générer un rapport</h3>
      <form @submit.prevent="generateReport">
        <ReportTypeSelector v-model="reportType" />
        <DateRangePicker v-model="dateRange" />
        <MetricsSelector v-model="selectedMetrics" />
        <button type="submit">Générer</button>
      </form>
    </div>

    <div class="scheduled-reports">
      <h3>Rapports programmés</h3>
      <ScheduledReportsList
        :reports="scheduledReports"
        @edit="editSchedule"
        @delete="deleteSchedule"
      />
    </div>

    <div class="report-history">
      <h3>Historique des rapports</h3>
      <ReportHistory
        :reports="reportHistory"
        @download="downloadReport"
        @delete="deleteReport"
      />
    </div>
  </div>
</template>
```

## 8. Sécurité

### 8.1 Gestion des accès
```vue
<template>
  <div class="access-control">
    <div class="roles-manager">
      <h3>Rôles</h3>
      <RoleEditor
        :roles="roles"
        @add="addRole"
        @update="updateRole"
        @delete="deleteRole"
      />
    </div>

    <div class="permissions-manager">
      <h3>Permissions</h3>
      <PermissionMatrix
        :roles="roles"
        :permissions="permissions"
        @update="updatePermissions"
      />
    </div>

    <div class="user-roles">
      <h3>Utilisateurs</h3>
      <UserRoleManager
        :users="users"
        :roles="roles"
        @update="updateUserRole"
      />
    </div>
  </div>
</template>
```