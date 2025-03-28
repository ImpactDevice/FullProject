function toggleSidebar() {
    const sidebar = document.getElementById('sidebar');
    const main = document.getElementById('main');
    const openbtn = document.querySelector('.openbtn');
    if (sidebar.style.width === '250px') {
        sidebar.style.width = '0';
        main.style.marginLeft = '0';
        sidebar.classList.add('collapsed');
    } else {
        sidebar.style.width = '250px';
        main.style.marginLeft = '250px';
        sidebar.classList.remove('collapsed');
    }
}

function showContent(contentId) {
    const contents = document.querySelectorAll('.content');
    contents.forEach(content => {
        content.style.display = 'none';
    });
    document.getElementById(contentId).style.display = 'block';
}