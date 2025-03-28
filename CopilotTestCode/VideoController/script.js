document.addEventListener('DOMContentLoaded', () => {
    const video = document.getElementById('videoPlayer');
    const prevFrameButton = document.getElementById('prevFrame');
    const nextFrameButton = document.getElementById('nextFrame');
    const frameRate = 30; // Assuming the video has a frame rate of 30 fps

    prevFrameButton.addEventListener('click', () => {
        video.currentTime = Math.max(0, video.currentTime - 1 / frameRate);
    });

    nextFrameButton.addEventListener('click', () => {
        video.currentTime = Math.min(video.duration, video.currentTime + 1 / frameRate);
    });
})