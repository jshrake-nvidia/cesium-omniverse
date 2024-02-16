from typing import Any, List, Tuple

from typing import overload

class Asset:
    def __init__(self, *args, **kwargs) -> None: ...
    @property
    def asset_id(self) -> int: ...
    @property
    def asset_type(self) -> str: ...
    @property
    def attribution(self) -> str: ...
    @property
    def bytes(self) -> int: ...
    @property
    def date_added(self) -> str: ...
    @property
    def description(self) -> str: ...
    @property
    def name(self) -> str: ...
    @property
    def percent_complete(self) -> int: ...
    @property
    def status(self) -> str: ...

class AssetTroubleshootingDetails:
    def __init__(self, *args, **kwargs) -> None: ...
    @property
    def asset_exists_in_user_account(self) -> bool: ...
    @property
    def asset_id(self) -> int: ...

class Assets:
    def __init__(self, *args, **kwargs) -> None: ...
    @property
    def items(self) -> Any: ...
    @property
    def link(self) -> str: ...

class CesiumIonSession:
    def __init__(self, *args, **kwargs) -> None: ...
    def disconnect(self) -> None: ...
    def get_assets(self, *args, **kwargs) -> Any: ...
    def get_authorize_url(self) -> str: ...
    def get_connection(self, *args, **kwargs) -> Any: ...
    def get_profile(self, *args, **kwargs) -> Any: ...
    def get_tokens(self, *args, **kwargs) -> Any: ...
    def is_asset_list_loaded(self) -> bool: ...
    def is_connected(self) -> bool: ...
    def is_connecting(self) -> bool: ...
    def is_loading_asset_list(self) -> bool: ...
    def is_loading_profile(self) -> bool: ...
    def is_loading_token_list(self) -> bool: ...
    def is_profile_loaded(self) -> bool: ...
    def is_resuming(self) -> bool: ...
    def is_token_list_loaded(self) -> bool: ...
    def refresh_assets(self) -> None: ...
    def refresh_profile(self) -> None: ...
    def refresh_tokens(self) -> None: ...

class Connection:
    def __init__(self, *args, **kwargs) -> None: ...
    def get_access_token(self) -> str: ...
    def get_api_uri(self) -> str: ...

class ICesiumOmniverseInterface:
    def __init__(self, *args, **kwargs) -> None: ...
    def connect_to_ion(self) -> None: ...
    def create_token(self, arg0: str) -> None: ...
    def credits_available(self) -> bool: ...
    def credits_start_next_frame(self) -> None: ...
    def get_asset_token_troubleshooting_details(self, *args, **kwargs) -> Any: ...
    def get_asset_troubleshooting_details(self, *args, **kwargs) -> Any: ...
    def get_credits(self) -> List[Tuple[str, bool]]: ...
    def get_default_token_troubleshooting_details(self, *args, **kwargs) -> Any: ...
    def get_render_statistics(self, *args, **kwargs) -> Any: ...
    def get_server_path(self) -> str: ...
    def get_server_paths(self) -> List[str]: ...
    def get_session(self, *args, **kwargs) -> Any: ...
    def get_sessions(self, *args, **kwargs) -> Any: ...
    def get_set_default_token_result(self, *args, **kwargs) -> Any: ...
    def is_default_token_set(self) -> bool: ...
    def is_tracing_enabled(self) -> bool: ...
    def on_shutdown(self) -> None: ...
    def on_stage_change(self, arg0: int) -> None: ...
    def on_startup(self, arg0: str) -> None: ...
    def on_update_frame(self, arg0: List[ViewportPythonBinding], arg1: bool) -> None: ...
    def print_fabric_stage(self) -> str: ...
    def reload_tileset(self, arg0: str) -> None: ...
    def select_token(self, arg0: str, arg1: str) -> None: ...
    def specify_token(self, arg0: str) -> None: ...
    @overload
    def update_troubleshooting_details(self, arg0: str, arg1: int, arg2: int, arg3: int) -> None: ...
    @overload
    def update_troubleshooting_details(self, arg0: str, arg1: int, arg2: int, arg3: int, arg4: int) -> None: ...

class Profile:
    def __init__(self, *args, **kwargs) -> None: ...
    @property
    def id(self) -> int: ...
    @property
    def username(self) -> str: ...

class RenderStatistics:
    def __init__(self, *args, **kwargs) -> None: ...
    @property
    def culled_tiles_visited(self) -> int: ...
    @property
    def geometries_capacity(self) -> int: ...
    @property
    def geometries_loaded(self) -> int: ...
    @property
    def geometries_rendered(self) -> int: ...
    @property
    def materials_capacity(self) -> int: ...
    @property
    def materials_loaded(self) -> int: ...
    @property
    def max_depth_visited(self) -> int: ...
    @property
    def tiles_culled(self) -> int: ...
    @property
    def tiles_loaded(self) -> int: ...
    @property
    def tiles_loading_main(self) -> int: ...
    @property
    def tiles_loading_worker(self) -> int: ...
    @property
    def tiles_rendered(self) -> int: ...
    @property
    def tiles_visited(self) -> int: ...
    @property
    def tileset_cached_bytes(self) -> int: ...
    @property
    def triangles_loaded(self) -> int: ...
    @property
    def triangles_rendered(self) -> int: ...

class SetDefaultTokenResult:
    def __init__(self, *args, **kwargs) -> None: ...
    @property
    def code(self) -> int: ...
    @property
    def message(self) -> str: ...

class Token:
    def __init__(self, *args, **kwargs) -> None: ...
    @property
    def id(self) -> str: ...
    @property
    def is_default(self) -> bool: ...
    @property
    def name(self) -> str: ...
    @property
    def token(self) -> str: ...

class TokenTroubleshootingDetails:
    def __init__(self, *args, **kwargs) -> None: ...
    @property
    def allows_access_to_asset(self) -> bool: ...
    @property
    def associated_with_user_account(self) -> bool: ...
    @property
    def is_valid(self) -> bool: ...
    @property
    def show_details(self) -> bool: ...
    @property
    def token(self) -> Token: ...

class Viewport:
    height: float
    projMatrix: Matrix4d
    viewMatrix: Matrix4d
    width: float
    def __init__(self) -> None: ...

def acquire_cesium_omniverse_interface(
    plugin_name: str = ..., library_path: str = ...
) -> ICesiumOmniverseInterface: ...
def release_cesium_omniverse_interface(arg0: ICesiumOmniverseInterface) -> None: ...
