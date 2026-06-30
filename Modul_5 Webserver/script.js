// Navigation menu toggle for mobile
const hamburger = document.querySelector('.hamburger');
const navLinks = document.querySelector('.nav-links');

if(hamburger) {
    hamburger.addEventListener('click', () => {
        navLinks.style.display = navLinks.style.display === 'flex' ? 'none' : 'flex';
        if (navLinks.style.display === 'flex') {
            navLinks.style.flexDirection = 'column';
            navLinks.style.position = 'absolute';
            navLinks.style.top = '70px';
            navLinks.style.right = '20px';
            navLinks.style.background = 'rgba(11, 15, 25, 0.95)';
            navLinks.style.padding = '20px';
            navLinks.style.borderRadius = '10px';
            navLinks.style.border = '1px solid rgba(255, 255, 255, 0.1)';
        }
    });
}

// Reveal elements on scroll
function reveal() {
    var reveals = document.querySelectorAll(".reveal");

    for (var i = 0; i < reveals.length; i++) {
        var windowHeight = window.innerHeight;
        var elementTop = reveals[i].getBoundingClientRect().top;
        var elementVisible = 100;

        if (elementTop < windowHeight - elementVisible) {
            reveals[i].classList.add("active");
        }
    }
}

window.addEventListener("scroll", reveal);

// Trigger once on load
reveal();

// Navbar background styling on scroll
window.addEventListener("scroll", () => {
    const navbar = document.querySelector('.navbar');
    if (window.scrollY > 50) {
        navbar.style.background = 'rgba(11, 15, 25, 0.95)';
        navbar.style.boxShadow = '0 2px 10px rgba(0,0,0,0.3)';
    } else {
        navbar.style.background = 'rgba(11, 15, 25, 0.8)';
        navbar.style.boxShadow = 'none';
    }
});

// Smooth scroll implementation
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        e.preventDefault();
        
        // Hide mobile menu if open
        if (window.innerWidth <= 768 && navLinks.style.display === 'flex') {
            navLinks.style.display = 'none';
        }

        document.querySelector(this.getAttribute('href')).scrollIntoView({
            behavior: 'smooth'
        });
    });
});
