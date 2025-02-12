// Fonction pour ouvrir un modal avec animation
function openModal(modalId) {
    const modal = document.getElementById(modalId);
    modal.classList.add('open'); // Ajoute la classe 'open' pour lancer l'animation
}

// Fonction pour fermer un modal avec animation
function closeModal(modalId) {
    const modal = document.getElementById(modalId);
    modal.classList.remove('open'); // Enlève la classe 'open' pour cacher le modal
}

// Fermeture des modals quand on clique en dehors du contenu
window.onclick = function(event) {
    const modals = document.getElementsByClassName('modal');
    for (let i = 0; i < modals.length; i++) {
        if (event.target == modals[i]) {
            modals[i].classList.remove('open'); // Enlève la classe 'open' si on clique à l'extérieur du modal
        }
    }
}
