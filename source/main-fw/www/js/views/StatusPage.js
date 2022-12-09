import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.setTitle("Status");
    }

    onStatusUpdate(event) {
        let idUserInstruction = document.querySelector("#idUserInstruction");
        let idStatusCoordinate = document.querySelector("#idStatusCoordinate");
        let idBtCancel = document.querySelector("#idBtCancel");
        let idProgressBar = document.querySelector("#idProgressBar");
        let idAnimation = document.querySelector("#idAnimation");

        idUserInstruction.innerHTML = getControlPrettyStepString(event.detail.status.state);
        idStatusCoordinate.innerHTML = "[x: " + event.detail.status.x.toFixed(1) + ", z: " + event.detail.status.z.toFixed(1) + ", y: " + event.detail.status.y.toFixed(1) + "]";

        idBtCancel.disabled = event.detail.status.is_cancel_request;

        idProgressBar.style["width"] = String(event.detail.status.percent*100) + "%";

        idAnimation.style["display"] = (event.detail.status.state) != EControlState.unspecified ? "block" : "none";
        // console.log("status", event.detail.status);
    }

    async loaded() {
        document.addEventListener("status_update", this.onStatusUpdate);
        console.log("status loaded");
    }

    async unloaded() {
        document.removeEventListener("status_update", this.onStatusUpdate);
        console.log("status unloaded");
    }

    async getHtml() {
        return `
        <div id="idStatus" class="status_container">
            <div style="display: flex; flex-direction: row;">
                <div id="idAnimation" class="lds-dual-ring" style="margin-top: 10px;"></div>
                <div id="idUserInstruction" class="status_userinstruction">
                    ---
                </div>
            </div>
            <p class="status_cocktailname">Sex on the beach</b></p>
            <div class="status_progress_container">
                <div class="progress-bar" style="margin-right: 10px;">
                    <span id="idProgressBar" class="progress-bar-fill" style="width: 0%; display: none"></span>
                </div>
                <button id="idBtCancel" class="button-cancel" disabled>Cancel</button>
            </div>
            <p id="idStatusCoordinate" class="status_coordinate">[x: 0, z: 0, y: 0]</p>
        </div>
        `;
    }
}
