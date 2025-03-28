const express = require('express');
// const fs = require('fs').promises;
const fs = require('fs');
const fsp = require('fs').promises;
const path = require('path');

const csv = require('csv-parser');
// This is for running the Python/C++ Code
const { exec } = require('child_process'); 
const app = express();
const port = 3000;

// Serve static files from the 'public' directory
app.use(express.static('public'));
app.use(express.json()); // Middleware to parse JSON bodies

//OVerlay Graph potentially
//Axis labels

//Pop-up info

//========================SUPER IMPORTANT FOR LOADING FILES=========================
//Only Loading the the latest 10 test

//Some instruction for settings tab

//Syncing-up the video

//=======================FIX THE STYLING ON VIDEO ONLY TO MATCH THE OTHER GRAPH ONLY===================
//There is a current issue where if you where to load a chart then switch datasets it can't load
//the new dataset cause you have to clear all the canvas


//Try messing around with main page color background black/grey Big buttons white small buttons orange
//Meeting Notes
//Load Cell Placements
//Can I get the STM Board to work and is the sampling rate high enough
//Where to place all the electronics put it into box?? get 24-pin or so connector to simplifiy disassembly




//Switching Slug sphere (masses) Change the mass
//dis, energy AND force vs time
//Look into Chart.js plotly.js, googlechart, and D3.js
//In the settings allow them to change the mass that sphere and Slug are set too


//=========================CSV FORMAT=================================================
//row 1: Header/Name Of Dataset
//row 2: is the column a set of points or a Eq
//row 3+: Data/points/polycoeff
//Column 1: Time points
//Column 2+: Data columns

//========================FOR DRAGGABLE FRAME FOR DISPLAYING DATA==========================
//-Add: fullscreen, Minimize, and Close
//-Add: Way To Zoom In Specifiy a range and Increment forward and backward
//-Add: Toggle Different Pieces of Data ie Raw Data, Filtered, Idea (mgh) ect.
  //-Layout Options:
    //- 1: Sandbox, pan around in the Top Panel & resize & drag all of the options
    
    //- 3: Make 1 Large Column that is scrollable

    //- 2: Left hand side tabs for different options make the side tab colapsable.
    //This option is more-so to replace the current method for selecting Which way To display the Data

//======================Copilot UI Design Tip Notes==========================
//Stick to 2-3 font sizes
//Maintain consistent padding and margins
//minimalism
//Use white space effectively to guide users
//Hierarchy  Use different font sizes, weights, and styles to create a clear visual hierarchy.
//Add Loading Indicators
//More Icons
//



//==============CLEAN THIS UP AND FIX THE Try Statement==================
app.get('/Get-Preset-Test', async (req, res) => {
  const directoryPath = path.join(__dirname, 'Memory', 'SavedTest');
  try {
    // const files = await fs.readdir(directoryPath);
    // const directories = [];
    // for (const file of files) {
    //   const filePath = path.join(directoryPath, file);
    //   const stats = await fs.stat(filePath);
    //   if (stats.isDirectory()) {
    //     directories.push(file);
    //   }
    // }
    res.json(directories);
  } catch (err) {
    console.error('Error reading directory:', err);
    res.status(500).send('Error reading directory');
  }
});

app.get('/Get-Preset-Test-Names', async (req, res) => {
  const directoryPath = path.join(__dirname, 'Memory', 'SavedTest');
  try {
    const files = await fsp.readdir(directoryPath);
    const directories = [];
    for (const file of files) {
      const filePath = path.join(directoryPath, file);
      if (path.extname(filePath) === '.json') {
        directories.push(file);
      }
    }
    res.json(directories);
  } catch (err) {
    console.error('Error reading directory:', err);
    res.status(500).send('Error reading directory');
  }
});








// Endpoint to get subdirectories
app.get('/subdirectories/:Dir', async (req, res) => {
  const Dir = req.params.Dir;
  const directoryPath = path.join(__dirname, Dir);

  try {
    const directories = getSortedSubdirectories(directoryPath);
    res.json(directories);
  } catch (err) {
    console.error('Error reading directory:', err);
    res.status(500).send('Error reading directory');
  }
});
// This should sort to make it so that the most recent Dataset is first (maybe last)
function getSortedSubdirectories(directoryPath) {
  // Read the contents of the directory
  const subdirectories = fs.readdirSync(directoryPath).filter(file => {
      return fs.statSync(path.join(directoryPath, file)).isDirectory();
  });

  // Get the stats for each subdirectory and sort by mtime (modified time)
  const sortedSubdirectories = subdirectories.map(subdirectory => {
      const fullPath = path.join(directoryPath, subdirectory);
      return {
          name: subdirectory,
          time: fs.statSync(fullPath).mtime.getTime()
      };
  }).sort((a, b) => b.time - a.time).map(subdirectory => subdirectory.name);

  return sortedSubdirectories;
}




















//function to grab everything inside a specific Dataset
async function readDataset(Dataset_Path) {
    const items = await fs.readdir(Dataset_Path, { withFileTypes: true});
    const result = [];

    for (const item of items) {
        const fullPath = path.join(directoryPath, item.name);
        if (item.isDirectory()) {
          result.push({
            name: item.name,
            type: 'directory',
            children: await readDirectory(fullPath)
          });
        } else {
          result.push({
            name: item.name,
            type: 'file'
          });
        }
      }

    return result;
}

// Endpoint to get everything inside a Dataset
// app.get('/Dataset-files/:dir', async (req, res) => {
//     const dirName = req.params.dir;
//     const dirPath = path.join(__dirname, 'CollectedData', dirName);
// })

app.get('/GetCSV/:Dataset/:filename', (req, res) => {
  const results = [];
  const SetName = req.params.Dataset;
  const filename = req.params.filename;
  fs.createReadStream(path.join(__dirname, 'CollectedData', SetName, filename))
    .pipe(csv())
    .on('data', (data) => results.push(data))
    .on('end', () => {
      res.json(results);
    });
});

// Endpoint to serve a specific SVG file
//IDK if this works like this /data/:Dataset/:filename
app.get('/CollectedData/:Dataset/:filename', (req, res) => {
    const SetName = req.params.Dataset;
    const filename = req.params.filename;
    const filePath = path.join(__dirname, 'CollectedData', SetName, filename);
    res.sendFile(filePath);
});


//Heres an example of what we need to write to run the C++ Scripts
//We will basically need a function & File for each Operation
app.post('/run-motor', (req, res) => {
    const { Dir, Dis } = req.body;
    const command = `./your_script ${Dir} ${Dis}`; //Might add Alpha here or just read it from the Settings inside the .cpp File

    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${error.message}`);
            return res.status(500).send(error.message);
        }
        if (stderr) {
            console.error(`Stderr: ${stderr}`);
            return res.status(500).send(stderr);
        }
        res.send(stdout);
    });
});

app.get('/Settings', (req, res) => {
  const filePath = path.join(__dirname, 'Memory', 'Settings', 'Setting.json');
  res.sendFile(filePath);
});

app.get('/:Dataset/Info', (req, res) => {
  const SetName = req.params.Dataset;
  const filePath = path.join(__dirname, 'CollectedData', SetName, 'Info.json');
  res.sendFile(filePath);
});

// Endpoint to save JSON data
app.post('/save-settings', async (req, res) => {
  const jsonData = req.body;
  const filePath = path.join(__dirname, 'Memory', 'Settings', 'Setting.json');

  try {
    await fsp.writeFile(filePath, JSON.stringify(jsonData, null, 2));
    res.send('Data saved successfully');
  } catch (err) {
    console.error('Error writing file:', err);
    res.status(500).send('Error saving data');
  }
});

app.post('/save-Preset-Test/:TestName', async (req, res) => {
  const TestName = req.params.TestName + ".json";
  const jsonData = req.body;
  const filePath = path.join(__dirname, 'Memory', 'SavedTest', TestName);

  try {
    await fsp.writeFile(filePath, JSON.stringify(jsonData, null, 2));
    res.send('Data saved successfully');
  } catch (err) {
    console.error('Error writing file:', err);
    res.status(500).send('Error saving data');
  }
});

app.get('/AllData/:Dataset', (req, res) => {
  const SetName = req.params.Dataset;
  const directoryPath = path.join(__dirname, 'CollectedData', SetName);
  const results = { csvFiles: [], jsonFile: null };

  // Read CSV files
  const csvFiles = ['FakeDistanceData1.csv', 'file2.csv', 'ForceSensorData.csv'];
  let csvReadCount = 0;

  csvFiles.forEach((file) => {
    const filePath = path.join(directoryPath, file);

    // Check if the file exists
    if (fs.existsSync(filePath)) {
      const fileData = [];
      fs.createReadStream(filePath)
        .pipe(csv())
        .on('data', (data) => fileData.push(data))
        .on('end', () => {
          results.csvFiles.push({ fileName: file, data: fileData });
          csvReadCount++;
          if (csvReadCount === csvFiles.length) {
            // Read JSON file after all CSV files are read
            const jsonFilePath = path.join(directoryPath, 'Info.json');
            fs.readFile(jsonFilePath, 'utf8', (err, jsonData) => {
              if (err) {
                res.status(500).send('Error reading JSON file');
                return;
              }
              results.jsonFile = JSON.parse(jsonData);
              res.json(results);
            });
          }
        });
    } else {
      csvReadCount++;
      if (csvReadCount === csvFiles.length) {
        // Read JSON file after all CSV files are read
        const jsonFilePath = path.join(directoryPath, 'Info.json');
        fs.readFile(jsonFilePath, 'utf8', (err, jsonData) => {
          if (err) {
            res.status(500).send('Error reading JSON file');
            return;
          }
          results.jsonFile = JSON.parse(jsonData);
          res.json(results);
        });
      }
    }
  });
});




app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});





//========================Stuff From Copilot To Use Later========================
// Endpoint to shut down the Raspberry Pi
//Run Command Line commands
app.post('/shutdown', (req, res) => {
  const command = 'sudo shutdown -h now';

  exec(command, (error, stdout, stderr) => {
      if (error) {
          console.error(`Error: ${error.message}`);
          return res.status(500).send(error.message);
      }
      if (stderr) {
          console.error(`Stderr: ${stderr}`);
          return res.status(500).send(stderr);
      }
      res.send('Shutdown command executed successfully');
  });
});


// const { parse } = require('csv-parse');
// // Function to read and parse a CSV file
// function readCSVFile(filePath, callback) {
//   const results = [];
//   fs.createReadStream(filePath)
//       .pipe(parse({ columns: true }))
//       .on('data', (data) => results.push(data))
//       .on('end', () => {
//           callback(null, results);
//       })
//       .on('error', (err) => {
//           callback(err, null);
//       });
// }

// // Endpoint to get CSV data
// app.get('/csv-data', (req, res) => {
//   const filePath = path.join(__dirname, 'path_to_your_csv_file.csv');
//   readCSVFile(filePath, (err, data) => {
//       if (err) {
//           console.error('Error reading CSV file:', err);
//           return res.status(500).send('Error reading CSV file');
//       }
//       res.json(data);
//   });
// });














//=========================================WORKING ON GET ALL DATA IN DATASET=========================




app.get('/AllData2/:Dataset', async (req, res) => {
  const SetName = req.params.Dataset;
  const directoryPath = path.join(__dirname, 'CollectedData', SetName);
  const results = { csvFiles: {}, jsonFile: null };

  // Read CSV files
  const csvFiles = ['FakeDistanceData1.csv', 'ForceSensorData.csv', 'File.csv'];

  try {
    const csvPromises = csvFiles.map(file => readCsvFile(path.join(directoryPath, file)));
    const csvData = await Promise.all(csvPromises);

    csvData.forEach((data, index) => {
      if (data) {
        // results.csvFiles.push({ fileName: csvFiles[index], data });
        results.csvFiles[csvFiles[index]] = data;
      }
    });

    // Read JSON file
    const jsonFilePath = path.join(directoryPath, 'Info.json');
    const jsonData = await readJsonFile(jsonFilePath);
    results.jsonFile = jsonData;

    res.json(results);
  } catch (error) {
    res.status(500).send('Error reading files');
  }
});

// function readCsvFile(filePath) {
//   return new Promise((resolve, reject) => {
//     if (!fs.existsSync(filePath)) {
//       resolve(null);
//       return;
//     }

//     const fileData = [];
//     fs.createReadStream(filePath)
//       .pipe(csv())
//       .on('data', (data) => fileData.push(data))
//       .on('end', () => resolve(fileData))
//       .on('error', (error) => reject(error));
//   });
// }

function readCsvFile2(filePath) {
  return new Promise((resolve, reject) => {
    if (!fs.existsSync(filePath)) {
      resolve(null);
      return;
    }

    const fileData = [];
    const headers = [];
    const types = [];
    const patterns = [];

    fs.createReadStream(filePath)
      .pipe(csv())
      .on('headers', (headerList) => {
        headers.push(...headerList);
      })
      .on('data', (data) => {
        if (fileData.length === 0) {
          // First row after headers contains types
          types.push(...Object.values(data));
        } else if (fileData.length === 1) {
          // Second row after headers contains patterns
          patterns.push(...Object.values(data));
        } else {
          // Subsequent rows contain actual data
          fileData.push(data);
        }
      })
      .on('end', () => {
        const result = {};
        headers.forEach((header, index) => {
          result[header] = {
            Type: types[index],
            Pattern: patterns[index] || '',
            Data: fileData.map(row => parseFloat(row[header]))
          };
        });
        resolve(result);
      })
      .on('error', (error) => reject(error));
  });
}

//Works like a charm assuming we format our .csv Files correctly
function readCsvFile(filePath) {
  return new Promise((resolve, reject) => {
    if (!fs.existsSync(filePath)) {
      resolve(null);
      return;
    }

    const fileData = [];
    const headers = [];
    const types = [];
    const patterns = [];
    let rowIndex = 0;

    fs.createReadStream(filePath)
      .pipe(csv())
      .on('headers', (headerList) => {
        headers.push(...headerList);
      })
      .on('data', (data) => {
        if (rowIndex === 0) {
          // First row after headers contains types
          types.push(...Object.values(data));
        } else if (rowIndex === 1) {
          // Second row after headers contains patterns
          patterns.push(...Object.values(data));
        } else {
          // Subsequent rows contain actual data
          fileData.push(data);
        }
        rowIndex++;
      })
      .on('end', () => {
        const result = {};
        headers.forEach((header, index) => {
          result[header] = {
            Type: types[index],
            Pattern: patterns[index] || '',
            Data: fileData.map(row => parseFloat(row[header]))
          };
        });
        resolve(result);
      })
      .on('error', (error) => reject(error));
  });
}

function readJsonFile(filePath) {
  return new Promise((resolve, reject) => {
    fs.readFile(filePath, 'utf8', (err, jsonData) => {
      if (err) {
        reject(err);
        return;
      }
      resolve(JSON.parse(jsonData));
    });
  });
}





















