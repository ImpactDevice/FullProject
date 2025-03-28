const { app, BrowserWindow } = require('electron');
const path = require('path');
const { exec } = require('child_process');

let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      nodeIntegration: true,
    },
  });

  mainWindow.loadURL('http://localhost:3000'); // Adjust the port if necessary

  mainWindow.on('closed', function () {
    mainWindow = null;
  });
}

app.on('ready', () => {
  exec('node server.js', (err, stdout, stderr) => {
    if (err) {
      console.error(`Error starting server: ${stderr}`);
      return;
    }
    console.log(`Server started: ${stdout}`);
    createWindow();
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});