setInterval(async function () {
  return await new Promise((resolve, reject) => {
    let xhr = new XMLHttpRequest();
    xhr.open('GET', '/flama');
    xhr.onload = function () {
      if (xhr.status === 200) {
        let data = xhr.responseText;
        console.log(data);
        resolve(data);
      }
      else {
        console.log('Error en la petición');
        reject('error');
      }
    };
  }, 4000);
})
  // Código que se ejecutará cada segundo
