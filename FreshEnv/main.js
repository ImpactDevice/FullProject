const { app, BrowserWindow } = require('electron');
const path = require('path');
const { spawn } = require('child_process');

function createWindow() {
  const mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    kiosk: true,
    frame: false,
    fullscreen: true,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: true,
    },
  });

  mainWindow.loadURL('http://localhost:3000');
}

// app.whenReady().then(() => {
//     console.log("Starting Server");
//   exec('node server.js', (err, stdout, stderr) => {
//     if (err) {
//         console.log("Error Opening Server");
//         console.log(err);
//       console.error(`Error starting server: ${err}`);
//       return;
//     } else {
//         console.log("No Errors Opening Server");
//     }
//     createWindow();
//     console.log(stdout);
//     console.error(stderr);
//     createWindow();
//   });

//   app.on('activate', () => {
//     if (BrowserWindow.getAllWindows().length === 0) createWindow();
//   });
// });

app.whenReady().then(() => {
    console.log("Starting Server");
    const server = spawn('node', ['server.js']);
  
    server.stdout.on('data', (data) => {
      console.log(`Server stdout: ${data}`);
    });
  
    server.stderr.on('data', (data) => {
      console.error(`Server stderr: ${data}`);
    });
  
    server.on('close', (code) => {
      console.log(`Server process exited with code ${code}`);
    });
  
    createWindow();
  
    app.on('activate', () => {
      if (BrowserWindow.getAllWindows().length === 0) createWindow();
    });
  });

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});