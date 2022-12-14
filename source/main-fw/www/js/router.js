import ListCocktailPage from "./views/ListCocktailPage.js";
import CustomCocktailPage from "./views/CustomCocktailPage.js";
import AboutPage from "./views/AboutPage.js";
import NetworkPage from "./views/NetworkPage.js";
import CalibPage from "./views/CalibPage.js";
import StatusPage from "./views/StatusPage.js";
import StationSettingsPage from "./views/StationSettingsPage.js";

/* https://dcode.domenade.com/tutorials/build-a-single-page-app-with-javascript-no-frameworks */

const pathToRegex = path => new RegExp("^" + path.replace(/\//g, "\\/").replace(/:\w+/g, "(.+)") + "$");

const getParams = match => {
    const values = match.result.slice(1);
    const keys = Array.from(match.route.path.matchAll(/:(\w+)/g)).map(result => result[1]);

    return Object.fromEntries(keys.map((key, i) => {
        return [key, values[i]];
    }));
};

const navigateTo = url => {
    history.pushState(null, null, url);
    router();
};

window.navigateTo = navigateTo;

let m_currentView = null;

const router = async () => {
    const routes = [
        { path: "/", view: StatusPage },
        { path: "/status", view: StatusPage },
        { path: "/network", view: NetworkPage },
        { path: "/calib", view: CalibPage },
        { path: "/stationsettings", view: StationSettingsPage },
        { path: "/customcocktail", view: CustomCocktailPage },
        { path: "/customcocktail/:id", view: CustomCocktailPage },
        { path: "/listcocktailpage", view: ListCocktailPage },
        { path: "/about", view: AboutPage },
    ];

    // Test each route for potential match
    const potentialMatches = routes.map(route => {
        return {
            route: route,
            result: location.pathname.match(pathToRegex(route.path))
        };
    });

    let match = potentialMatches.find(potentialMatch => potentialMatch.result !== null);

    if (!match) {
        match = {
            route: routes[0],
            result: [location.pathname]
        };
    }

    const view = new match.route.view(getParams(match));
    if (m_currentView) {
        if (m_currentView.unloaded) {
            await m_currentView.unloaded();
        }
        m_currentView = null;
    }

    document.querySelector("#app").innerHTML = await view.getHtml();
    if (view.loaded) {
        await view.loaded();
    }
    m_currentView = view;
};

window.addEventListener("popstate", router);

document.addEventListener("DOMContentLoaded", () => {
    document.body.addEventListener("click", e => {
        if (e.target.matches("[data-link]")) {
            e.preventDefault();
            navigateTo(e.target.href);
        }
    });

    router();
});
