const lightbulb = document.querySelector('.lightbulb');
const light = document.querySelector('.light');
const button = document.querySelector('.button-text');
const container = document.querySelector('.container');
const encender = document.getElementById('encender');
lightbulb.style.animationName = 'lightbulbOff';
light.style.animationName = 'lightOff';
button.textContent = 'Encender Foco';
container.classList.remove('bg-light');
container.classList.add('bg-dark');


function toggleLight() {
  if (lightbulb.style.animationName === 'lightbulbOff') {
    lightbulb.style.animationName = 'lightbulbOn';
    light.style.animationName = 'lightOn';
    button.textContent = 'Apagar Foco';
    container.classList.remove('bg-dark');
    container.classList.add('bg-light');
  } else {
    lightbulb.style.animationName = 'lightbulbOff';
    light.style.animationName = 'lightOff';
    button.textContent = 'Encender Foco';
    container.classList.remove('bg-light');
    container.classList.add('bg-dark');
  }

  try {
    fetch('/publish')
      .then(response => response.json())
      .then(data => console.log(data));
  } catch (error) {
    console.error(error);
  }
}