import logging
import carb.events
import omni.kit.app as app
import omni.ui as ui
from typing import List, Optional
from ..bindings import ICesiumOmniverseInterface
from .models import IonAssets, IonAssetItem, IonAssetDelegate
from .styles import CesiumOmniverseUiStyles


class CesiumOmniverseAssetWindow(ui.Window):
    """
    The asset list window for Cesium for Omniverse. Docked in the same area as "Assets".
    """

    WINDOW_NAME = "Cesium Assets"
    MENU_PATH = f"Window/Cesium/{WINDOW_NAME}"

    def __init__(self, cesium_omniverse_interface: ICesiumOmniverseInterface, **kwargs):
        super().__init__(CesiumOmniverseAssetWindow.WINDOW_NAME, **kwargs)

        self._cesium_omniverse_interface = cesium_omniverse_interface
        self._logger = logging.getLogger(__name__)

        self._assets = IonAssets()
        self._assets_delegate = IonAssetDelegate()

        self._refresh_button: Optional[ui.Button] = None
        self._asset_tree_view: Optional[ui.TreeView] = None

        self._subscriptions: List[carb.events.ISubscription] = []
        self._setup_subscriptions()

        self.frame.set_build_fn(self._build_fn)

        self._refresh_list()

        self.focus()

    def destroy(self):
        self._refresh_button.destroy()
        self._refresh_button = None

        self._asset_tree_view.destroy()
        self._asset_tree_view = None

        for subscription in self._subscriptions:
            subscription.unsubscribe()
        self._subscriptions.clear()

        super().destroy()

    def _setup_subscriptions(self):
        bus = app.get_app().get_message_bus_event_stream()

        assets_updated_event = carb.events.type_from_string("cesium.omniverse.ASSETS_UPDATED")
        self._subscriptions.append(
            bus.create_subscription_to_pop_by_type(assets_updated_event, self._on_assets_updated,
                                                   name="cesium.omniverse.asset_window.assets_updated")
        )

    def _refresh_list(self):
        session = self._cesium_omniverse_interface.get_session()

        if session is not None:
            self._logger.info("Cesium ion Assets refreshing.")
            session.refresh_assets()

    def _on_assets_updated(self, _e: carb.events.IEvent):
        session = self._cesium_omniverse_interface.get_session()

        if session is not None:
            self._logger.info("Cesium ion Assets refreshed.")
            self._assets.replace_items(
                [
                    IonAssetItem(
                        item.asset_id,
                        item.name,
                        item.description,
                        item.attribution,
                        item.asset_type,
                        item.date_added) for item in session.get_assets().items
                ]
            )

    def _refresh_button_clicked(self):
        self._refresh_list()

    def _selection_changed(self, items: List[IonAssetItem]):
        pass

    def _build_fn(self):
        """Builds all UI components."""

        with ui.VStack(spacing=5):
            with ui.HStack(height=30):
                self._refresh_button = ui.Button("Refresh", alignment=ui.Alignment.CENTER, width=80,
                                                 style=CesiumOmniverseUiStyles.blue_button_style,
                                                 clicked_fn=self._refresh_button_clicked)
                ui.Spacer()
            with ui.HStack(spacing=5):
                with ui.ScrollingFrame(style_type_name_override="TreeView",
                                       style={"Field": {"background_color": 0xFF000000}},
                                       width=ui.Length(2, ui.UnitType.FRACTION)):
                    self._asset_tree_view = ui.TreeView(self._assets, delegate=self._assets_delegate,
                                                        root_visible=False,
                                                        header_visible=True,
                                                        style={"TreeView.Item": {"margin": 4}})
                    self._asset_tree_view.set_selection_changed_fn(self._selection_changed)
                with ui.ScrollingFrame(width=ui.Length(1, ui.UnitType.FRACTION)):
                    with ui.VStack():
                        ui.Label("TODO: Selection Frame")
