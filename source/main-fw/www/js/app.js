async function timerHandler() {

    // Get system informations
    await fetch('/api/getstatus')
        .then((response) => {
            if (!response.ok) {
                throw new Error('Unable to process the order');
            }
            return response.json();
        })
        .then((data) =>
        {
            //console.log("data", data);
            document.dispatchEvent(new CustomEvent('status_update', {
                detail: { status : data },
                cancelable: true
              }));

            setTimeout(timerHandler, 500);
        })
        .catch((ex) =>
        {
            setTimeout(timerHandler, 5000);
            console.error('getstatus', ex);
        });
}

window.addEventListener("load", (event) => {
    console.log("page is fully loaded");

    setTimeout(timerHandler, 500);
});

