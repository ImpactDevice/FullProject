const windowElement = document.querySelector('.window');
const topbar = document.querySelector('.topbar');

topbar.addEventListener('mousedown', dragMouseDown);
topbar.addEventListener('touchstart', dragTouchStart);

function dragMouseDown(e) {
    e.preventDefault();
    startDrag(e.clientX, e.clientY);
    document.addEventListener('mousemove', dragMouseMove);
    document.addEventListener('mouseup', stopDrag);
}

function dragTouchStart(e) {
    e.preventDefault();
    const touch = e.touches[0];
    startDrag(touch.clientX, touch.clientY);
    document.addEventListener('touchmove', dragTouchMove);
    document.addEventListener('touchend', stopDrag);
}

function startDrag(clientX, clientY) {
    const rect = windowElement.getBoundingClientRect();
    windowElement.offsetX = clientX - rect.left;
    windowElement.offsetY = clientY - rect.top;
}

function dragMouseMove(e) {
    moveWindow(e.clientX, e.clientY);
}

function dragTouchMove(e) {
    const touch = e.touches[0];
    moveWindow(touch.clientX, touch.clientY);
}

function moveWindow(clientX, clientY) {
    windowElement.style.left = (clientX - windowElement.offsetX) + 'px';
    windowElement.style.top = (clientY - windowElement.offsetY) + 'px';
}

function stopDrag() {
    document.removeEventListener('mousemove', dragMouseMove);
    document.removeEventListener('mouseup', stopDrag);
    document.removeEventListener('touchmove', dragTouchMove);
    document.removeEventListener('touchend', stopDrag);
}