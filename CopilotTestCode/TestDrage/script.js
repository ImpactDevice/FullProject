let isPanning = false;
let startX, startY, initialLeft, initialTop;
let initialDistance = 0;
let initialScale = 1;
let currentScale = 1;

const container = document.getElementById('container');
const largeDiv = document.getElementById('large-div');

container.addEventListener('touchstart', function(e) {
    if (e.touches.length === 1) {
        isPanning = true;
        startX = e.touches[0].clientX;
        startY = e.touches[0].clientY;
        initialLeft = largeDiv.offsetLeft;
        initialTop = largeDiv.offsetTop;
    } else if (e.touches.length === 2) {
        isPanning = false;
        initialDistance = getDistance(e.touches[0], e.touches[1]);
        initialScale = currentScale;
    }
});

container.addEventListener('touchmove', function(e) {
    if (isPanning && e.touches.length === 1) {
        const dx = e.touches[0].clientX - startX;
        const dy = e.touches[0].clientY - startY;
        largeDiv.style.left = initialLeft + dx + 'px';
        largeDiv.style.top = initialTop + dy + 'px';
    } else if (e.touches.length === 2) {
        const newDistance = getDistance(e.touches[0], e.touches[1]);
        const scale = (newDistance / initialDistance) * initialScale;
        currentScale = scale;
        largeDiv.style.transform = `scale(${scale})`;
    }
});

container.addEventListener('touchend', function(e) {
    if (e.touches.length === 0) {
        isPanning = false;
    }
});

function getDistance(touch1, touch2) {
    const dx = touch2.clientX - touch1.clientX;
    const dy = touch2.clientY - touch1.clientY;
    return Math.sqrt(dx * dy + dy * dy);
}