// Sample data array






window.onload = function() {
    //This is the Size of our Touchscreen Display
    //it is also a 17.3 inch Display with a 16:9 Ascpect Ratio
    window.resizeTo(1920, 1080);
    // window.fileList = document.getElementById('Recent-Data-Options-List');
    
    
    // Make_Buttons();
  
    // Input_Test_Btn();
  
    // Data_Display_Options_Close_Btn();
    // Make_Open_VDF1_Btn();
    // Setup_Close_Data_Display();
    // Setup_Change_Display_Type_Btn();
    // Setup_Settings_Btn();
    // Setup_Close_All_Top_Displays_Btn();
    Setup_Draggable_Windows();
    Make_Graph1();

  }


function polynomial(x) {
    return Math.sin( 10/ Math.PI * x);
}


function Make_Graph1() {
    // Generate x and y values
    const xValues = [];
    const yValues = [];
    for (let x = 0; x <= 2 * Math.PI; x += 0.01) {
        xValues.push(x);
        yValues.push(polynomial(x));
    }

    // Create the chart
    const ctx = document.getElementById('myEnergyChart').getContext('2d');
    const myChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: xValues,
            datasets: [{
                label: 'y = sin(500 * π * x)',
                data: yValues,
                backgroundColor: 'rgba(75, 192, 192, 0.2)',
                borderColor: 'rgba(75, 192, 192, 1)',
                borderWidth: 1,
                fill: false
            }]
        },
        options: {
            scales: {
                x: {
                    type: 'linear',
                    title: {
                        display: true,
                        text: 'x'
                    },
                    min: 0,
                    max: 2 * Math.PI
                },
                y: {
                    title: {
                        display: true,
                        text: 'y'
                    },
                    beginAtZero: true
                }
            },
            plugins: {
                title: {
                    display: true,
                    text: 'Polynomial Equation Plot'
                }
            }
        }
    });
}

//Draggable Windows
function Setup_Draggable_Windows() {
    const divs = [
    //   { buttonId: 'showDivBtn1', divId: 'Video-Window' },
    //   { buttonId: 'showDivBtn2', divId: 'Distance-Window' },
      { buttonId: 'showDivBtn3', divId: 'Energy-Window' }
    //   { buttonId: 'showDivBtn3', divId: 'numberPad' }
    ];
    divs.forEach(({ divId }) => {
      const div = document.getElementById(divId);
      const dragHandle = div.querySelector('.Drag-Handle');
      let isDragging = false;
      let offsetX, offsetY;
      let initialDistance = 0;
      let initialWidth = 0;
      let initialHeight = 0;
    
      dragHandle.addEventListener('mousedown', function(e) {
          isDragging = true;
          offsetX = e.clientX - div.offsetLeft;
          offsetY = e.clientY - div.offsetTop;
      });
    
      // div.addEventListener('mousedown', function() {
      //   div.style.zIndex = ++highestZIndex; // Bring to top
      // });
  
      document.addEventListener('mousemove', function(e) {
          if (isDragging) {
              div.style.left = `${e.clientX - offsetX}px`;
              div.style.top = `${e.clientY - offsetY}px`;
          }
      });
    
      document.addEventListener('mouseup', function() {
          isDragging = false;
      });
    
      div.addEventListener('touchstart', function(e) {
          if (e.touches.length === 2) {
              initialDistance = getDistance(e.touches[0], e.touches[1]);
              initialWidth = div.offsetWidth;
              initialHeight = div.offsetHeight;
          }
      });
    
      div.addEventListener('touchmove', function(e) {
          if (e.touches.length === 2) {
              const currentDistance = getDistance(e.touches[0], e.touches[1]);
              const scale = currentDistance / initialDistance;
              div.style.width = `${initialWidth * scale}px`;
              div.style.height = `${initialHeight * scale}px`;
          }
      });
    
      div.addEventListener('touchend', function(e) {
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



    // function resizeCanvas(Parent) {
    //     const Identifier = Parent + " .Window-Body .panel canvas";
    //     console.log(Identifier);
    //     const canvas = document.querySelector('.panel canvas');
    //     const parent = canvas.parentElement;
    //     canvas.width = parent.clientWidth;
    //     canvas.height = parent.clientHeight;
    // }
    
    // document.getElementById('Energy-Window').addEventListener('resize', resizeCanvas('Energy-Window'));
  

    // function resizeCanvas2() {
    //     const canvas = document.querySelector('.panel canvas');
    //     const parent = canvas.parentElement;
    //     canvas.width = parent.clientWidth;
    //     canvas.height = parent.clientHeight;
    // }
    // window.addEventListener('resize', resizeCanvas2);
  }



// function resizeCanvas() {
//     const resizableElements = document.querySelectorAll('.resizable');
//     resizableElements.forEach(resizable => {
//         const canvas = resizable.querySelector('canvas');
//         if (canvas) {
//             canvas.width = resizable.clientWidth;
//             canvas.height = resizable.clientHeight;
//         }
//     });
// }

// window.addEventListener('resize', resizeCanvas);



// // Resize observer to handle .resizable element resizing
// const resizeObserver = new ResizeObserver(entries => {
//     for (let entry of entries) {
//         const canvas = entry.target.querySelector('canvas');
//         if (canvas) {
//             canvas.width = entry.contentRect.width;
//             canvas.height = entry.contentRect.height;
//         }
//     }
// });

// const resizableElements = document.querySelectorAll('.resizable');
// resizableElements.forEach(resizable => {
//     resizeObserver.observe(resizable);
// });


