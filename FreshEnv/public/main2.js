//=====Common Classes List=============
//top-Close-Btn To close ALL divs with class Top

// To Bring them To A different HTML Page is
//window.location.href = 'File.html';

//=================EventListeners====================

window.onload = function () {
  //This is the Size of our Touchscreen Display
  //it is also a 17.3 inch Display with a 16:9 Ascpect Ratio
  window.resizeTo(1920, 1080);
  window.DatasetFileList = document.getElementById("Recent-Data-Options-List");
  window.PresetFileList = document.getElementById("Preset-Test-List");
  window.Charts = {};
  window.Charts_Original_Config = {};
  // window.SensorDatas = {};
  window.Datasets = {};
  window.ActiveChartMaker = {};
  window.Framerate = 960; //Will need to change this when a dataset gets loaded in from Info.json
  //The Info.json should be just the Testings Test and maybe a couple more Pieces of info

  window.Gradient1 = "rgb(75, 192, 192)";
  window.Gradient2 = "rgb(77, 75, 192)";

  Make_Dataset_Buttons();
  Make_Preset_Test_Buttons();

  Input_Test_Btn();
  Manual_Control_Button();

  Setup_Close_Data_Display();
  // Setup_Change_Display_Type_Btn();
  Setup_Settings_Btn();
  Setup_Close_All_Top_Displays_Btn();
  Setup_Draggable_Windows();
  // feather.replace();
};

// async function listDirectories(directoryPath) {
//   try {
//     const files = await fs.readdir(directoryPath);
//   for (const file of files) {
//     const filePath = path.join(directoryPath, file);
//     const stats = await fs.stat(filePath);
//     if (stats.isDirectory()) {
//       console.log('Directory:', file);
//     }
//   }
//   } catch (err) {
//     console.error('Error reading directory:', err);
//   }
// }

// function Get_Settings_File() {
//   return '/Settings';
// }

// function Insert_Current_Settings(Settings) {
//   const Motor_Alpha_Input = document.getElementById('Motor-Alpha-Value-Input');
//   Motor_Alpha_Input.value = Settings.Calibration.Motor.Alpha;
// }

// function Load_Current_Settings() {
//   const Response = Get_Settings_File();
//   const Settings = Response.json();
//   console.log(Settings);
//   Insert_Current_Settings(Settings);
// }

// async function Open_Settings() {
//   Load_Current_Settings();
//   document.getElementById("Settings-Top").classList.remove("Hidden");
// }

// function Open_Settings() {
//   fetch('/Settings')
//     .then(response => response.json())
//     .then(Settings => {
//       console.log(Settings);
//       const Motor_Alpha_Input = document.getElementById('Motor-Alpha-Value-Input');
//       Motor_Alpha_Input.value = Settings.Calibration.Motor.Alpha;
//       document.getElementById("Settings-Top").classList.remove("Hidden");
//     });
// }

async function fetchSettings() {
  const response = await fetch("/Settings");
  const settings = await response.json();
  window.SavedSettings = settings;
}

async function updateUI() {
  const Motor_Alpha_Input = document.getElementById("Motor-Alpha-Value-Input");
  Motor_Alpha_Input.value = window.SavedSettings.Calibration.Motor.Alpha;
  document.getElementById("Settings-Top").classList.remove("Hidden");
}

async function Open_Settings() {
  await fetchSettings();
  await updateUI();
}

async function Read_Test_Input() {
  const Form = document.getElementById("Input-Test-Form");
  const formData = new FormData(Form);
  window.Test_Inputs = {};
  formData.forEach((value, key) => {
    window.Test_Inputs[key] = value;
  });
}

async function Save_Test_To_Server() {
  try {
    const response = await fetch(
      `/save-Preset-Test/${window.Test_Inputs["Test-Name"]}`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(window.Test_Inputs),
      }
    );

    if (response.ok) {
      console.log("Data saved successfully");
    } else {
      console.error("Error saving data");
    }
  } catch (error) {
    console.error("Error:", error);
  }
}

//Need to refresh the visible Preset Test to show the new one
async function Save_Preset_Test() {
  await Read_Test_Input();
  await Save_Test_To_Server();
  Close_Input_Test_Window();
}

async function Read_Settings_Inputs() {
  const Form = document.getElementById("Input-Test-Form");
  const formData = new FormData(Form);
  let data = {};
  formData.forEach((value, key) => {
    data[key] = value;
  });
  console.log(data);
}

async function Save_Settings_To_Server() {
  try {
    const response = await fetch("/save-settings", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(window.SavedSettings),
    });

    if (response.ok) {
      console.log("Data saved successfully");
    } else {
      console.error("Error saving data");
    }
  } catch (error) {
    console.error("Error:", error);
  }
}

async function Save_Settings() {
  await Read_Settings_Inputs();
  await Save_JSON_To_Server();
  Close_Settings();
}

function Close_Settings() {
  document.getElementById("Settings-Top").classList.add("Hidden");
}

function Setup_Settings_Btn() {
  document
    .getElementById("Open-Settings-Btn")
    .addEventListener("click", Open_Settings);
  document
    .getElementById("Save-Settings-Test")
    .addEventListener("click", Save_Settings);
}

// function Setup_Change_Display_Type_Btn() {
//   document.getElementById("Change-Data-Display-Type-Btn").addEventListener('click', Back_To_Display_Type);
// }

function Back_To_Display_Type() {
  Close_All_Top();
  Show_Data_Display_Options();
}

function Setup_Close_Data_Display() {
  const Return_Btns = document.getElementsByClassName("Data-Display-Close-Btn");
  let i = 0;
  while (Return_Btns.length > i) {
    Return_Btns[i].addEventListener("click", Close_All_Top);
    i = i + 1;
  }
}

function Setup_Close_All_Top_Displays_Btn() {
  const Return_Btns = document.getElementsByClassName("Top-Close-Btn");
  let i = 0;
  while (Return_Btns.length > i) {
    Return_Btns[i].addEventListener("click", Close_All_Top);
    i = i + 1;
  }
}

function Close_All_Top() {
  const elements = document.getElementsByClassName("Top");
  let i = 0;
  while (elements.length > i) {
    elements[i].classList.add("Hidden");
    i = i + 1;
  }
}

//===========================DATA-DISPLAY STUFF MOSTLY===========================

function Open_VDF1() {
  Open_VDF1_Data();
  Open_VDF1_Window();
  Hide_Data_Display_Options();
}

function Make_Open_VDF1_Btn() {
  document.getElementById("VDF-1-Btn").addEventListener("click", Open_VDF1);
}

function Data_Display_Options_Close_Btn() {
  document
    .getElementById("Display-Data-Options-Close-Btn")
    .addEventListener("click", Hide_Data_Display_Options);
}

function Open_Input_Test_Window() {
  const Panel = document.getElementById("Input-Test-Top");
  Panel.classList.remove("Hidden");
}

function Close_Input_Test_Window() {
  const Panel = document.getElementById("Input-Test-Top");
  Panel.classList.add("Hidden");
}

function Input_Test_Btn() {
  document
    .getElementById("Input-Test-Button")
    .addEventListener("click", Open_Input_Test_Window);
  document
    .getElementById("Close-Input-Test")
    .addEventListener("click", Close_Input_Test_Window);
}

function Open_Manual_Control_Window() {
  const Panel = document.getElementById("Manual-Control-Top");
  Panel.classList.remove("Hidden");
}

function Manual_Control_Button() {
  const Btn = document.getElementById("Manual-Control-Btn");
  Btn.addEventListener("click", Open_Manual_Control_Window);
}

//This can probably be cleaned up
function Get_yValues_And_Labels() {
  const CurrentCSV = window.ActiveChartMaker.CurrentCSV;
  const Sensor_Data = window.Datasets[window.cDataset].csvFiles[CurrentCSV];
  window.ActiveChartMaker.xValues = Sensor_Data.Time.Data;
  window.ActiveChartMaker.yLabels = [];
  window.ActiveChartMaker.yValues = {};
  for (const key in Sensor_Data) {
    if (key != "Time") {
      window.ActiveChartMaker.yLabels.push(key);
      if (Sensor_Data[key].Type == "Eq") {
        const Samples = SampleEq(
          Sensor_Data[key].Data,
          Sensor_Data.Time.Data,
          Sensor_Data[key].Pattern
        );
        window.ActiveChartMaker.yValues[key] = Samples;
      } else {
        window.ActiveChartMaker.yValues[key] = Sensor_Data[key].Data;
      }
    }
  }
}

//We Can Also Use This To Display That Max Value Force of Impact or whatever
//This is Inclusing Time
function Find_Y_Limits() {
	const CurrentCSV = window.ActiveChartMaker.CurrentCSV;
	const obj = window.Datasets[window.cDataset].csvFiles[CurrentCSV];
	let allData = [];
	for (let key in obj) {
		if (key == "Time") {
		continue;
		}
		if (obj[key].Type !== "Eq") {
		allData = allData.concat(obj[key].Data);
		}
	}
	let max = Math.max(...allData);
	let min = Math.min(...allData);

	return { min, max };
}

//This is to basiclly to give the graph some padding on the actual graph
//I'm assuming the Max is never negative
function Properly_Scale_Y_Limits(Limits) {
	const max = Limits.max * 1.03;
	let min = 0;
	if (Limits.min > 0) {
		min = Limits.min * 0.97;
	} else if (Limits.min < 0) {
		min = Limits.min * 1.03;
	} else {
		min = -1 * Limits.max * 0.03;
	}
	return { min, max };
}

function RandomRGB() {
	const Range = 50;
	let Old_rgb = window.Gradient1;
	if (Math.random() <= 0.5) {
		Old_rgb = window.Gradient2;
	}
	const rgb = Old_rgb.match(/\d+/g).map(Number);

	const offset = () => Math.floor(Math.random() * Range * 2 + 1) - Range;

	const newRgb = rgb.map((component) =>
		Math.max(0, Math.min(255, component + offset()))
	);

	return `rgb(${newRgb.join(", ")})`;
}

function Make_Data_For_Chart_Config() {
	Get_yValues_And_Labels();
	let Y_Limits = Find_Y_Limits();
	Y_Limits = Properly_Scale_Y_Limits(Y_Limits);

	let Datasets = [];
	let i = 0;
	//This Can All be cleaned up Later
	// Colors = ['rgb(204, 147, 4)', 'rgb(161, 151, 0)', 'rgb(255, 182, 47)', 'rgb(204, 111, 4)', 'rgb(253, 243, 100)','rgb(204, 147, 4)'];
	// Colors_Bg = ['rgb(204, 147, 4)', 'rgb(161, 151, 0)', 'rgb(255, 182, 47)', 'rgb(204, 111, 4)', 'rgb(253, 243, 100)','rgb(204, 147, 4)'];

	const yLabels = window.ActiveChartMaker.yLabels;
	const yValues = window.ActiveChartMaker.yValues;
	const xValues = window.ActiveChartMaker.xValues;
	while (i < yLabels.length) {
		let Color = RandomRGB();
		let yValue = yValues[yLabels[i]];
		let obj = {
		label: yLabels[i],
		data: yValue,
		// backgroundColor: 'rgba(75, 192, 192, 0.2)',
		// borderColor: 'rgba(75, 192, 192, 1)',
		backgroundColor: Color,
		borderColor: Color,
		borderWidth: 1,
		fill: false,
		hidden: false,
		};
		Datasets.push(obj);
		i++;
	}

	console.log(Datasets);

	const Config = {
		type: "line",
		data: {
		labels: xValues,
		datasets: Datasets,
		},

		options: {
		responsive: true,
		maintainAspectRatio: false,
		scales: {
			x: {
			type: "linear",
			title: {
				display: true,
				text: "x",
			},
			min: xValues[0],
			max: xValues[-1],
			},
			y: {
			title: {
				display: true,
				text: "y",
			},
			beginAtZero: true,
			min: Y_Limits.min,
			max: Y_Limits.max,
			},
		},
		plugins: {
			title: {
			display: true,
			text: "Sensor Data", //We Can Name it Later Probably When we deal with the Data's File Names
			},
			zoom: {
			pan: {
				enabled: true,
				mode: "xy",
				speed: 10,
				threshold: 10,
			},
			zoom: {
				enabled: true,
				drag: false,
				mode: "xy",
				wheel: {
				enabled: true,
				},
				pinch: {
				enabled: true,
				},
				limits: {
				max: 10,
				min: 0.5,
				},
			},
			},
		},
		},
	};
	return Config;
}

function Make_Graph_Only_Content() {
	const ChartID = window.ActiveChartMaker.ChartID;
	const Canvas = document.getElementById(ChartID);
	const ctx = Canvas.getContext("2d");
	window.Charts[ChartID] = {};
	window.Charts[ChartID].Canvas = Canvas;
	const Configure = Make_Data_For_Chart_Config();
	window.Charts[ChartID].Chart = new Chart(ctx, Configure);
	window.Charts_Original_Config[ChartID] = deepCopy(Configure);
	Setup_yLabel_Toggles();
}

const GraphTypeTocsvFile = {
	"Energy-Only": "FakeDistanceData1.csv",
	"Force-Only": "ForceSensorData.csv",
	"Distance-Only": "EnergyData.csv",
	"FakeDistanceData1.csv": "Energy-Only",
	"ForceSensorData.csv": "Force-Only",
	"EnergyData.csv": "Distance-Only",
};

function Make_Data_Display_Content() {
  //The Current Dataset Should Already have been Set globally

	const HTML_Name = GraphTypeTocsvFile[window.ActiveChartMaker.CurrentCSV];
	const ChartID = HTML_Name + "-Chart";
	window.ActiveChartMaker.ChartID = ChartID;
	const Chart = window.Charts[ChartID];
	Make_Graph_Only_Content();
  // if(Chart == null) {
  //   const temp = 7;
  //   //Display Some kind of Error message
  //   // Make_Graph_Only_Content(Display_Type_ID);
  // }
}

function Clear_All_Charts() {
	const Charts = window.Charts;
	console.log(Charts);
	for (const key in Charts) {
		window.Charts[key].Chart.destroy();
	}
}

function Remove_All_Decendents(Selector) {
	const parentElm = document.querySelectorAll(Selector);
	parentElm.forEach((elm) => {
		while (elm.firstChild) {
		elm.removeChild(elm.firstChild);
		}
	});
}

function Clear_Old_Data() {
	Clear_All_Charts();
	Remove_All_Decendents(".Graph-Toggle-Switches-Container");
	Remove_All_Decendents(".Graph-Info-Container");
}

function Load_Video() {
  const vidContainer = document.getElementById("Video-Div");
  vidContainer.innerHTML = `<video controls><source src="/CollectedData/${window.cDataset}/TestVideo.mp4" type="video/mp4"></video>`;
}

//I'm Still not correctly destroying the old Canvas
function Load_All_Data_Graphs() {
	Clear_Old_Data();
  	const CSVs = window.Datasets[window.cDataset].csvFiles;

	for (const key in CSVs) {
		window.ActiveChartMaker.CurrentCSV = key;
		Make_Data_Display_Content();
	}
	Load_Video();
}

function Show_Data_Display_Options() {
	const Panel = document.getElementById("Data-Display-Top");
  	Panel.classList.remove("Hidden");
}

async function DisplayDataOptions(BtnID) {
  window.DataBtnID = BtnID;
  window.cDataset = BtnID;
  await Open_All_Dataset_Files(BtnID);
  Load_All_Data_Graphs();
  Show_Data_Display_Options();
}

function Make_Dataset_Buttons() {
  fetch("/subdirectories/CollectedData")
    .then((response) => response.json())
    .then((files) => {
      window.DatasetFileList.innerHTML = ""; // Clear the list
      files.forEach((file) => {
        const listItem = document.createElement("li");
        const ItemBtn = document.createElement("button");
        listItem.id = file + "-Container";
        listItem.className = "Recent-Data-Btn-Container";
        ItemBtn.id = file + "-Btn";
        ItemBtn.className = "Recent-Folder";
        ItemBtn.textContent = file;
        ItemBtn.classList.add("Btn-Style-6");
        ItemBtn.addEventListener("click", () => {
          DisplayDataOptions(file);
        });
        listItem.appendChild(ItemBtn);
        window.DatasetFileList.appendChild(listItem);
      });
    })
    .catch((error) => console.error("Error fetching files:", error));
}

function Run_Test(File) {
  const Temp = 7; // Ill write this later
}

function Make_Preset_Test_Buttons() {
  fetch("/Get-Preset-Test-Names")
    .then((response) => response.json())
    .then((files) => {
      window.PresetFileList.innerHTML = ""; // Clear the list
      files.forEach((file) => {
        const listItem = document.createElement("li");
        const ItemBtn = document.createElement("button");
        listItem.id = file + "-Container";
        listItem.className = "Recent-Data-Btn-Container";
        ItemBtn.id = file + "-Btn";
        ItemBtn.className = "Recent-Folder";
        ItemBtn.textContent = file;
        ItemBtn.classList.add("Btn-Style-6");
        ItemBtn.addEventListener("click", () => {
          Run_Test(file);
        });
        listItem.appendChild(ItemBtn);
        window.PresetFileList.appendChild(listItem);
      });
    })
    .catch((error) => console.error("Error fetching files:", error));
}

function DisplayDataBtn(BtnID) {
  // OpenSelectedData(BtnID);
  Show_Data_Display_Options();
}

function Hide_Data_Display_Options() {
  const Panel = document.getElementById("Data-Display-Options-Top");
  Panel.classList.add("Hidden");
}

function AddGraph1() {
  const Parent = document.getElementById("Distance-Container");
  const NewImg = document.createElement("img");
  const svgContainer = document.getElementById("Distance-Container");
  svgContainer.innerHTML = `<img src="/CollectedData/${window.DataBtnID}/FakeData1.svg">`;
}

function AddGraph2() {
  const svgContainer = document.getElementById("Energy-Container");
  svgContainer.innerHTML = `<img src="/CollectedData/${window.DataBtnID}/FakeData2.svg">`;
}

function AddVideo() {
  const vidContainer = document.getElementById("Video-Div");
  vidContainer.innerHTML = `<video controls><source src="/CollectedData/${window.DataBtnID}/TestVideo.mp4" type="video/mp4"></video>`;
}

function Open_VDF1_Data() {
  AddGraph1();
  AddGraph2();
  AddVideo();
}

//=============================Function For New Data Display Method W/ Sidebar=============================
//Going to Need A function that automatically collects all the data from a datasets and a way of knowing whats all in it
//ie is there video data or not, also is the graph data an eq or Array, Also a way of changing the sampling rate and range of eq data

function toggleSidebarOLD() {
  const sidebar = document.getElementById("Data-Display-Sidebar");
  const main = document.getElementById("Data-Display-Main");
  const openbtn = document.querySelector(".Open-Sidebar-Btn");
  if (sidebar.classList.contains("collapsed")) {
    sidebar.style.width = "250px";
    main.style.marginLeft = "250px";
    sidebar.classList.remove("collapsed");
  } else {
    sidebar.style.width = "250px";
    main.style.marginLeft = "0";
    sidebar.classList.add("collapsed");
  }
}

function toggleSidebar() {
  const sidebar = document.getElementById("Data-Display-Sidebar");
  const main = document.getElementById("Data-Display-Main");
  const openbtn = document.querySelector(".Open-Sidebar-Btn");
  if (sidebar.classList.contains("collapsed")) {
    sidebar.classList.remove("collapsed");
    main.classList.remove("expanded");
    // setTimeout(() => {s}) Use this if only using Fixed doesn't work also do hidden not none
    openbtn.style.display = "none";
  } else {
    sidebar.classList.add("collapsed");
    main.classList.add("expanded");
    openbtn.style.display = "block";
  }
}

function ToggleInfoDiv(Div) {
  const InfoDiv = document.getElementById(Div);
  if (InfoDiv.classList.contains("Hidden")) {
    InfoDiv.classList.remove("Hidden");
    // openbtn.style.display = 'none';
  } else {
    InfoDiv.classList.add("Hidden");
    // openbtn.style.display = 'block';
  }
}

function deepCopy(obj) {
  if (obj === null || typeof obj !== "object") {
    return obj;
  }

  if (Array.isArray(obj)) {
    return obj.map(deepCopy);
  }

  const copy = {};
  for (let key in obj) {
    if (Object.prototype.hasOwnProperty.call(obj, key)) {
      copy[key] = deepCopy(obj[key]);
    }
  }
  return copy;
}

function Get_Data_Labels(obj) {
  yLabels = [];
  for (const key in obj) {
    if (key != "Time") {
      yLabels.push(key);
    }
  }
  return yLabels;
}

function SampleEq(Eq, Time, degree) {
  let i = 0;
  console.log(Eq);
  data = [];
  while (i < Time.length) {
    let Point = 0;
    let j = 0;
    while (j <= degree) {
      Point = Point + Eq[j] * Time[i] ** (degree - j);
      j++;
    }
    // Eq.forEach((Coeff) => {
    //   console.log(Coeff);
    //   Point = Point + Coeff * (Time[i] ** (degree-i));
    // });
    data.push(Point);
    i++;
  }
  return data;
}

//We Can Also Use This To Display That Max Value Force of Impact or whatever
//This is Inclusing Time
// function Find_Y_Limits() {
//   const CurrentCSV = window.ActiveChartMaker.CurrentCSV;
//   const obj = window.Datasets[window.cDataset].csvFiles[CurrentCSV];
//   let allData = [];
//   console.log("Prior to For loop");
//   console.log(obj);
//   for (let key in obj) {
//     if(key == 'Time') {
//       continue;
//     }
//     if (obj[key].Type !== 'Eq') {
//       allData = allData.concat(obj[key].Data);
//     }
//   }
//   console.log(allData);
//   let max = Math.max(...allData);
//   let min = Math.min(...allData);

//   max = max*1.03;
//   min = min*1.03;

//   console.log(CurrentCSV);
//   console.log(max);

//   return { min, max };
// }

// function Get_yValues_And_Labels() {
//   const CurrentCSV = window.ActiveChartMaker.CurrentCSV;
//   const Sensor_Data = window.Datasets[window.cDataset].csvFiles[CurrentCSV];
//   window.ActiveChartMaker.xValues = Sensor_Data.Time.Data;
//   window.ActiveChartMaker.yLabels = [];
//   window.ActiveChartMaker.yValues = {};
//   for (const key in Sensor_Data) {
//     if(key != "Time") {
//       window.ActiveChartMaker.yLabels.push(key);
//       if(Sensor_Data[key].Type == "Eq") {
//         const Samples = SampleEq(Sensor_Data[key].Data, Sensor_Data.Time.Data, Sensor_Data[key].Pattern);
//         window.ActiveChartMaker.yValues[key] = Samples;
//       } else {
//         window.ActiveChartMaker.yValues[key] = Sensor_Data[key].Data;
//       }
//     }
//   }
// }

// function Make_Data_For_Chart_Config() {

//   Get_yValues_And_Labels();
//   const Y_Limits = Find_Y_Limits();

//   let Datasets = [];
//   let i = 0;
//   Colors = ['rgb(204, 147, 4)', 'rgb(161, 151, 0)', 'rgb(255, 182, 47)', 'rgb(204, 111, 4)', 'rgb(253, 243, 100)','rgb(204, 147, 4)'];
//   Colors_Bg = ['rgb(204, 147, 4)', 'rgb(161, 151, 0)', 'rgb(255, 182, 47)', 'rgb(204, 111, 4)', 'rgb(253, 243, 100)','rgb(204, 147, 4)'];
//   const yLabels = window.ActiveChartMaker.yLabels;
//   const yValues = window.ActiveChartMaker.yValues;
//   const xValues = window.ActiveChartMaker.xValues;
//   while (i < yLabels.length) {
//     let yValue = yValues[yLabels[i]];
//     let obj = {
//       label: yLabels[i],
//       data: yValue,
//       backgroundColor: 'rgba(75, 192, 192, 0.2)',
//       // borderColor: 'rgba(75, 192, 192, 1)',
//       borderColor: Colors[i],
//       borderWidth: 1,
//       fill: false,
//       hidden: false
//     }
//     Datasets.push(obj);
//     i++;
//   }

//   console.log(Datasets);

//   const Config = {
//     type: "line",
//     data: {
//       labels: xValues,
//       datasets: Datasets,
//     },

//     options: {
//       responsive: true,
//       maintainAspectRatio: false,
//       scales: {
//           x: {
//               type: 'linear',
//               title: {
//                   display: true,
//                   text: 'x'
//               },
//               min: xValues[0],
//               max: xValues[-1]
//           },
//           y: {
//               title: {
//                   display: true,
//                   text: 'y'
//               },
//               beginAtZero: true,
//               min: Y_Limits.min,
//               max: Y_Limits.max
//           }
//       },
//       plugins: {
//           title: {
//               display: true,
//               text: 'Polynomial Equation Plot'
//           },
//           zoom: {
//             pan: {
//                 enabled: true,
//                 mode: "xy",
//                 speed: 10,
//                 threshold: 10
//             },
//             zoom: {
//                 enabled: true,
//                 drag: false,
//                 mode: "xy",
//                 wheel: {
//                     enabled: true
//                 },
//                 pinch: {
//                     enabled: true
//                 },
//                 limits: {
//                     max: 10,
//                     min: 0.5
//                 }
//             }
//         }
//       }
//    }
//   }
//   return Config;
// }

function Make_Graph_Config_Obj(DataType) {
  const Configure = {
    type: "line",
    data: {
      labels: xValues,
      datasets: [
        {
          label: "y = sin(x)",
          data: yValues,
          backgroundColor: "rgba(75, 192, 192, 0.2)",
          // borderColor: 'rgba(75, 192, 192, 1)',
          borderColor: "rgb(204, 147, 4)",
          borderWidth: 1,
          fill: false,
        },
        {
          label: "y = x/10",
          data: yValues2,
          backgroundColor: "rgba(75, 192, 192, 0.2)",
          // borderColor: 'rgb(75, 83, 192)',
          borderColor: "rgb(232, 183, 58)",
          borderWidth: 1,
          fill: false,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      scales: {
        x: {
          type: "linear",
          title: {
            display: true,
            text: "x",
          },
          min: 0,
          max: 2 * Math.PI,
        },
        y: {
          title: {
            display: true,
            text: "y",
          },
          beginAtZero: true,
        },
      },
      plugins: {
        title: {
          display: true,
          text: "Polynomial Equation Plot",
        },
        zoom: {
          pan: {
            enabled: true,
            mode: "x",
            speed: 10,
            threshold: 10,
          },
          zoom: {
            enabled: true,
            drag: false,
            mode: "xy",
            wheel: {
              enabled: true,
            },
            pinch: {
              enabled: true,
            },
            limits: {
              max: 10,
              min: 0.5,
            },
          },
        },
      },
    },
  };
}

// function Make_Buttons() {
//   fetch('/subdirectories')
//     .then(response => response.json())
//     .then(files => {
//       window.fileList.innerHTML = ''; // Clear the list
//       files.forEach(file => {
//         const listItem = document.createElement('li');
//         const ItemBtn = document.createElement('button');
//         listItem.id = file + "-Container";
//         listItem.className = "Recent-Data-Btn-Container";
//         ItemBtn.id = file + "-Btn";
//         ItemBtn.className = "Recent-Folder";
//         ItemBtn.textContent = file;
//         ItemBtn.classList.add("Btn-Style-5");
//         ItemBtn.addEventListener('click', () => {
//           DisplayDataOptions(file);
//         });
//         listItem.appendChild(ItemBtn);
//         fileList.appendChild(listItem);

function Make_Switch_HTML(index) {
  const Wrapper = document.createElement("div");
  Wrapper.className = "Switch-Wrapper-Style-2";
  const Box = document.createElement("div");
  Box.className = "Switch-Box-Style-2";
  const Container = document.createElement("div");
  Container.className = "Switch-Container-Style-2";
  const Switch_Input = document.createElement("input");
  Switch_Input.type = "checkbox";
  Switch_Input.checked = true;
  // const Dataset = window.Charts[window.ActiveChartMaker.ChartID].Chart.config.data.datasets[window.ActiveChartMaker.yIndex++];
  const Dataset = window.cToggleData;
  const Chart = window.Charts[window.ActiveChartMaker.ChartID].Chart;
  Switch_Input.addEventListener("click", () => {
    Dataset.hidden = !Dataset.hidden;
    Chart.update();
  });
  Container.appendChild(Switch_Input);
  Box.appendChild(Container);
  Wrapper.appendChild(Box);
  return Wrapper;
}

//For Right Now I'm Assuming I know the Parent Element
function Make_Label_Toggle_HTML() {
  //Making the Test
  const Graph_Body =
    window.Charts[window.ActiveChartMaker.ChartID].Canvas.parentElement
      .parentElement;
  const Header_Selector =
    "#" + Graph_Body.id + " .Graph-Header .Graph-Toggle-Switches-Container";
  console.log(Header_Selector);
  const Header = document.querySelector(Header_Selector);
  const TextElm = document.createElement("div");
  TextElm.className = "Graph-Toggle-Text";
  // const Label = window.ActiveChartMaker.cLabels;
  const Label = window.cToggleData.label;
  TextElm.ID = Label;
  TextElm.textContent = Label;
  //Making the Switch
  const Wrapper = Make_Switch_HTML();
  //Maybe Add A container here to hold these 2 elms together
  Header.appendChild(TextElm);
  Header.appendChild(Wrapper);
}

function Setup_yLabel_Toggles() {
  // window.ActiveChartMaker.yIndex = 0;
  // while(window.ActiveChartMaker.yIndex < window.ActiveChartMaker.yLabels.length) {
  //   window.ActiveChartMaker.cLabels = window.ActiveChartMaker.yLabels[window.ActiveChartMaker.yIndex];
  //   Make_Label_Toggle_HTML();
  //   window.ActiveChartMaker.yIndex++;
  // }
  const Datasets =
    window.Charts[window.ActiveChartMaker.ChartID].Chart.config.data.datasets;
  Datasets.forEach((Dataset) => {
    window.cToggleData = Dataset;
    Make_Label_Toggle_HTML();
  });
  // window.ActiveChartMaker.yLabels.forEach((Label) => {
  //   window.ActiveChartMaker.cLabel = Label;
  //   Make_Label_Toggle_HTML();
  // });
}

// const GraphTypeTocsvFile = {
//   "Energy-Only": 'FakeDistanceData1.csv',
//   "Force-Only": 'ForceSensorData.csv',
//   "Distance-Only": 'EnergyData.csv',
//   "FakeDistanceData1.csv": "Energy-Only",
//   "ForceSensorData.csv": "Force-Only",
//   "EnergyData.csv": 'Distance-Only'
// }

// function Make_Graph_Only_Content(Display_Type_ID) {
//   //Display_Type_ID Should Be The Parent Elm To the Graph IE Energy-Only
//   const ChartID = Display_Type_ID + "-Chart";
//   window.ActiveChartMaker.ChartID = ChartID;
//   const selector = "#" + Display_Type_ID + " div div canvas";
//   const ctx = document.querySelector(selector).getContext('2d');
//   window.Charts[ChartID] = {};
//   window.Charts[ChartID].Canvas = document.querySelector(selector);
//   const xValues = Get_xValues();
//   const yValues = Get_yValues();
//   const yValues2 = Get_yValues2();
//   const ConfigureStatic = {
//     type: 'line',
//     data: {
//         labels: xValues,
//         datasets: [{
//             label: 'y = sin(x)',
//             data: yValues,
//             backgroundColor: 'rgba(75, 192, 192, 0.2)',
//             // borderColor: 'rgba(75, 192, 192, 1)',
//             borderColor: 'rgb(204, 147, 4)',
//             borderWidth: 1,
//             fill: false
//         },{
//           label: 'y = x/10',
//           data: yValues2,
//           backgroundColor: 'rgba(75, 192, 192, 0.2)',
//           // borderColor: 'rgb(75, 83, 192)',
//           borderColor: 'rgb(232, 183, 58)',
//           borderWidth: 1,
//           fill: false
//       }]
//     },
//     options: {
//         responsive: true,
//         maintainAspectRatio: false,
//         scales: {
//             x: {
//                 type: 'linear',
//                 title: {
//                     display: true,
//                     text: 'x'
//                 },
//                 min: 0,
//                 max: 2 * Math.PI
//             },
//             y: {
//                 title: {
//                     display: true,
//                     text: 'y'
//                 },
//                 beginAtZero: true
//             }
//         },
//         plugins: {
//             title: {
//                 display: true,
//                 text: 'Polynomial Equation Plot'
//             },
//             zoom: {
//               pan: {
//                   enabled: true,
//                   mode: "x",
//                   speed: 10,
//                   threshold: 10
//               },
//               zoom: {
//                   enabled: true,
//                   drag: false,
//                   mode: "xy",
//                   wheel: {
//                       enabled: true
//                   },
//                   pinch: {
//                       enabled: true
//                   },
//                   limits: {
//                       max: 10,
//                       min: 0.5
//                   }
//               }
//           }
//         }
//     }
// };
//   window.ActiveChartMaker.CurrentCSV = GraphTypeTocsvFile[Display_Type_ID];
//   window.ActiveChartMaker.ChartID = Display_Type_ID + "-Chart";
//   const Configure = Make_Data_For_Chart_Config();
//   window.Charts[ChartID].Chart = new Chart(ctx, Configure);
//   window.Charts_Original_Config[ChartID] = deepCopy(Configure);
//   Setup_yLabel_Toggles();
// }

function ResetGraphRange(ChartID) {
  const Chart = window.Charts[ChartID].Chart;
  const Chart_Config = window.Charts_Original_Config[ChartID];
  Chart.config.options.scales.x.min = Chart_Config.options.scales.x.min;
  Chart.config.options.scales.x.max = Chart_Config.options.scales.x.max;
  Chart.config.options.scales.y.min = Chart_Config.options.scales.y.min;
  Chart.config.options.scales.y.max = Chart_Config.options.scales.y.max;
  Chart.update();
}

function ChangeGraphRange(ChartID, FooterID) {
  const Selector_Start = "#" + FooterID + " .Range-Start";
  const Selector_End = "#" + FooterID + " .Range-End";
  const Start_Input = document.querySelector(Selector_Start);
  const End_Input = document.querySelector(Selector_End);
  const Chart = window.Charts[ChartID].Chart;
  if (Start_Input.value > End_Input.value) {
    alert("Invalid Input: Start Value is Larger Than End Value");
  } else {
    Chart.config.options.scales.x.min = Start_Input.value;
    Chart.config.options.scales.x.max = End_Input.value;
    Chart.update();
  }
}

//This can be turned into a more General Function
function Setup_Energy_Only() {
  const Graph = document.getElementById("Energy-Only");
  // EnergyGraph.style.display = 'block';
  Graph.classList.remove("Hidden");
  Graph.classList.add("Data-Solo");
}

function Setup_Distance_Only() {
  const Graph = document.getElementById("Distance-Only");
  // EnergyGraph.style.display = 'block';
  Graph.classList.remove("Hidden");
  Graph.classList.add("Data-Solo");
}

function Setup_Force_Only() {
  const Graph = document.getElementById("Force-Only");
  // EnergyGraph.style.display = 'block';
  Graph.classList.remove("Hidden");
  Graph.classList.add("Data-Solo");
}

function Setup_Video_Only() {
  const Graph = document.getElementById("Video-Only");
  // EnergyGraph.style.display = 'block';
  Graph.classList.remove("Hidden");
  Graph.classList.add("Data-Solo");
}

function Setup_Quadrants() {
  let Displays = document.querySelectorAll(".Data-Display-Type-Top");
  Displays.forEach((display) => {
    display.classList.remove("Hidden");
    display.classList.add("Data-Quadrant");
  });
  document.getElementById("Data-Display-Main").classList.add("Quadrants-Style");
}

function Setup_Column() {
  let Displays = document.querySelectorAll(".Data-Display-Type-Top");
  Displays.forEach((display) => {
    display.classList.remove("Hidden");
    display.classList.add("Data-Column");
  });
  document.getElementById("Data-Display-Main").classList.add("Column-Style");
}

//Need to fixe the Size of the Chart Header's and Footers For things like Quadrants
const Setup_Data_Display_Style_Type = {
  Quadrants: Setup_Quadrants,
  Column: Setup_Column,
  // "Sandbox": Setup_Sandbox
  "Energy-Only": Setup_Energy_Only,
  "Distance-Only": Setup_Distance_Only,
  "Force-Only": Setup_Force_Only,
  "Video-Only": Setup_Video_Only,
};

function Show_Data_Display_Type(contentId) {
  //Makes The Specific Graph Type Visible
  toggleSidebar();
  const contents = document.querySelectorAll(".Data-Display-Type-Top");
  contents.forEach((content) => {
    // content.style.display = 'none';
    content.className = "Data-Display-Type-Top";
    content.classList.add("Hidden");
  });
  //Remove Previous Style Arrangement
  document.getElementById("Data-Display-Main").className =
    "Data-Display-Type-Top-Main";
  // document.getElementById(contentId).style.display = 'block';
  if (Setup_Data_Display_Style_Type[contentId]) {
    Setup_Data_Display_Style_Type[contentId]();
  } else {
    console.log("Display Type Doesnt Exist");
  }
}

function polynomial(x) {
  return Math.sin((10 / Math.PI) * x);
}

function polynomial2(x) {
  return 0.1 * x;
}

function Get_Data() {
  const xValues = [];
  const yValues = [];
  for (let x = 0; x <= 2 * Math.PI; x += 0.01) {
    xValues.push(x);
    yValues.push(polynomial(x));
  }
  return [xValues, yValues];
}

function Get_Data2() {
  const xValues = [];
  const yValues = [];
  for (let x = 0; x <= 2 * Math.PI; x += 0.01) {
    xValues.push(x);
    yValues.push(polynomial2(x));
  }
  return [xValues, yValues];
}

function Get_xValues() {
  const values = Get_Data();
  return values[0];
}

function Get_yValues() {
  const values = Get_Data();
  return values[1];
}

function Get_yValues2() {
  const values = Get_Data2();
  return values[1];
}

function Forward_Video() {
  const selector = "#Video-Div video";
  const video = document.querySelector(selector);
  video.currentTime = Math.min(
    video.duration,
    video.currentTime + 1 / window.Framerate
  );
}

function Backward_Video() {
  const selector = "#Video-Div video";
  const video = document.querySelector(selector);
  video.currentTime = Math.max(0, video.currentTime - 1 / window.Framerate);
}

//This Creates all the li elements
function listDirectories() {
  fetch("/subdirectories")
    .then((response) => response.json())
    .then((files) => {
      const fileList = document.getElementById("file-list");
      fileList.innerHTML = ""; // Clear the list
      files.forEach((file) => {
        const listItem = document.createElement("li");
        listItem.textContent = file;
        fileList.appendChild(listItem);
      });
    })
    .catch((error) => console.error("Error fetching files:", error));
}

//Draggable Windows
function Setup_Draggable_Windows() {
  const divs = [
    // { buttonId: 'showDivBtn1', divId: 'Video-Window' },
    // { buttonId: 'showDivBtn2', divId: 'Distance-Window' },
    // { buttonId: 'showDivBtn3', divId: 'Energy-Window' },
    { buttonId: "showDivBtn3", divId: "numberPad" },
  ];
  divs.forEach(({ divId }) => {
    const div = document.getElementById(divId);
    const dragHandle = div.querySelector(".Drag-Handle");
    let isDragging = false;
    let offsetX, offsetY;
    let initialDistance = 0;
    let initialWidth = 0;
    let initialHeight = 0;

    dragHandle.addEventListener("mousedown", function (e) {
      isDragging = true;
      offsetX = e.clientX - div.offsetLeft;
      offsetY = e.clientY - div.offsetTop;
    });

    // div.addEventListener('mousedown', function() {
    //   div.style.zIndex = ++highestZIndex; // Bring to top
    // });

    document.addEventListener("mousemove", function (e) {
      if (isDragging) {
        div.style.left = `${e.clientX - offsetX}px`;
        div.style.top = `${e.clientY - offsetY}px`;
      }
    });

    document.addEventListener("mouseup", function () {
      isDragging = false;
    });

    div.addEventListener("touchstart", function (e) {
      if (e.touches.length === 2) {
        initialDistance = getDistance(e.touches[0], e.touches[1]);
        initialWidth = div.offsetWidth;
        initialHeight = div.offsetHeight;
      }
    });

    div.addEventListener("touchmove", function (e) {
      if (e.touches.length === 2) {
        const currentDistance = getDistance(e.touches[0], e.touches[1]);
        const scale = currentDistance / initialDistance;
        div.style.width = `${initialWidth * scale}px`;
        div.style.height = `${initialHeight * scale}px`;
      }
    });

    div.addEventListener("touchend", function (e) {
      if (e.touches.length < 2) {
        initialDistance = 0;
      }
    });

    function getDistance(touch1, touch2) {
      const dx = touch2.clientX - touch1.clientX;
      const dy = touch2.clientY - touch1.clientY;
      return Math.sqrt(dx * dx + dy * dy);
    }
  });
}

//Keyboard & Number Pad
function showNumberPad(ID) {
  window.NumPadInputID = ID;
  const Numpad = (document.getElementById("numberPad").style.display = "flex");
  document.addEventListener("click", handleClickOutside, true);
}

function addNumber(num) {
  const input = document.getElementById(window.NumPadInputID);
  if (num === "." && input.value.includes(".")) return;
  input.value += num;
}

function clearInput() {
  document.getElementById(window.NumPadInputID).value = "";
}

function handleClickOutside(event) {
  const numberPad = document.getElementById("numberPad");
  const numberInput = document.getElementById(window.NumPadInputID);
  if (!numberPad.contains(event.target) && event.target !== numberInput) {
    numberPad.style.display = "none";
    // numberPad.style.top = 'auto';
    // numberPad.style.bottom = 0;
    // numberPad.style.left = 0;

    document.removeEventListener("click", handleClickOutside, true);
  }
}

function showKeyboard(ID) {
  window.KeyboardInputID = ID;
  window.isShiftActive = false;
  window.isCapsLockActive = false;
  window.is_Shift_Locked = false;
  document.getElementById("keyboard").style.display = "flex";
  document.addEventListener("click", handleClickOutsideKeyPad, true);
}

function addCharacter(Char) {
  const input = document.getElementById(window.KeyboardInputID);
  let Is_Shift = false;
  if (window.is_Shift_Locked) {
    Is_Shift = !window.isShiftActive;
  } else {
    Is_Shift = window.isShiftActive;
  }
  if (Is_Shift) {
    Char = Char.toUpperCase();
    Char = KeyboardConverter[Char] || Char;
  } else if (window.isCapsLockActive) {
    Char = Char.toUpperCase();
  } else {
    Char = Char.toLowerCase();
  }
  window.isShiftActive = false;
  updateShiftKey();
  input.value += Char;
}

function clearKeyboard() {
  document.getElementById(window.KeyboardInputID).value = "";
}

function deleteCharacter() {
  const KeyPad = document.getElementById(window.KeyboardInputID);
  KeyPad.value = KeyPad.value.slice(0, -1);
}

function toggleShift() {
  isShiftActive = !isShiftActive;
  updateShiftKey();
}

function toggleCapsLock() {
  isCapsLockActive = !isCapsLockActive;
  updateCapsLockKey();
}

function Toggle_Shift_Lock() {
  window.is_Shift_Locked = !window.is_Shift_Locked;
  Update_Shift_Lock_Key();
}

function updateShiftKey() {
  const shiftKey = document.getElementsByClassName("key__shift");
  Array.from(shiftKey).forEach((Key) => {
    if (window.isShiftActive) {
      Key.classList.add("Active-Key");
    } else {
      Key.classList.remove("Active-Key");
    }
  });
}

function updateCapsLockKey() {
  const capsLockKey = document.getElementsByClassName("key__caps");
  Array.from(capsLockKey).forEach((Key) => {
    if (window.isCapsLockActive) {
      Key.classList.add("Active-Key");
    } else {
      Key.classList.remove("Active-Key");
    }
  });
}

function Update_Shift_Lock_Key() {
  const ShiftLock = document.getElementsByClassName("key__shiftlock");
  Array.from(ShiftLock).forEach((Key) => {
    if (window.is_Shift_Locked) {
      Key.classList.add("Active-Key");
    } else {
      Key.classList.remove("Active-Key");
    }
  });
}

function handleClickOutsideKeyPad(event) {
  const KeyPad = document.getElementById("keyboard");
  const numberInput = document.getElementById(window.KeyboardInputID);
  if (!KeyPad.contains(event.target) && event.target !== numberInput) {
    KeyPad.style.display = "none";
    document.removeEventListener("click", handleClickOutsideKeyPad, true);
  }
}

const KeyboardConverter = {
  1: "!",
  2: "@",
  3: "#",
  4: "$",
  5: "%",
  6: "^",
  7: "&",
  8: "*",
  9: "(",
  0: ")",
  "-": "_",
  "=": "+",
  "[": "{",
  "]": "}",
  "\\": "|",
  ";": ":",
  "'": '"',
  ",": "<",
  ".": ">",
  "/": "?",
  "`": "~",
};

//====================NEW WAY OF DOING GRAPHS===================================

//=============================OPEN EVERYTHING INSIDE A DATASET FOLDER=============================

// function Open_All_Dataset_Files(Dataset) {
//   fetch(`/AllData2/${Dataset}`)
//   .then(response => response.json())
//   .then(data => {
//    window.Datasets[Dataset] = data;
//    console.log(window.Datasets[Dataset]);
//  })
//  .catch(error => console.error('Error fetching Dataset Info:', error));
// }

// async function Open_All_Dataset_Files(Dataset) {
//   let response = `/AllData2/${Dataset}`;
//   let data = response.json();
//   window.Datasets[Dataset] = data;
//   fetch(`/AllData2/${Dataset}`)
//   .then(response => response.json())
//   .then(data => {
//    window.Datasets[Dataset] = data;
//    console.log(window.Datasets[Dataset]);
//  })
//  .catch(error => console.error('Error fetching Dataset Info:', error));
// }

//async Copilot Version

async function Open_All_Dataset_Files(Dataset) {
  try {
    const response = await fetch(`/AllData2/${Dataset}`);
    if (!response.ok) {
      throw new Error("Error Opening Dataset Files");
    }
    const data = await response.json();
    window.Datasets[Dataset] = data;
  } catch (error) {
    console.error("Fetch Error: ", error);
  }
}

// async function Open_Dataset_Info_File() {
//    fetch(`/${window.cDataset}/Info`)
//    .then(response => response.json())
//    .then(data => {
//     window.Datasets[window.cDataset]['Info'] = {}; //This line might not be need
//     window.Datasets[window.cDataset].Info = data;
//   })
//   .catch(error => console.error('Error fetching Dataset Info:', error));
// }

// async function Open_All_Data_In_Dataset2() {
//   window.Datasets[window.cDataset] = {};
//   Open_All_Sensor_Data_Files(Dataset);
//   Open_Dataset_Info_File(Dataset);
// }

// async function Open_All_Data_In_Dataset() {
//   fetch(`/AllData/${window.cDataset}`)
//   .then(response => response.json())
//   .then(data => {
//    console.log(data);
//  })
//  .catch(error => console.error('Error fetching Dataset Info:', error));
// }

// data = {
//     "Time": {
//       "Type": "Points",
//       "Pattern": '',
//       "Data": [0, 0.012, ..]
//     },
//     "Ideal Distance": {
//       "Type": "Points",
//       "Pattern": '',
//       "Data": [3, 2.99, ..]
//     },
//     "Raw Distance": {
//       "Type": "Points",
//       "Pattern": '',
//       "Data": [2.9, 2.99, ..]
//     },
//     "Eq Distance": {
//       "Type": "Eq",
//       "Pattern": 'Degree 10',
//       "Data": [-2460.., 6125.., ..]
//     }
// }

//=============================SAVE A TEST CODE=============================

function Get_All_Inputted_Options() {
  let Saved_Settings = {};
  Saved_Settings.TestName = document.getElementById("Name-Input").textContent; //this might be wrong
}

function Save_Test() {
  Get_All_Inputted_Options();
  Write_Saved_Test_Config_JSON();
}
