/**
 * @file _module.ixx
 * @brief Aggregate module for `helios::engine::rendering::concepts`.
 */
export module helios.engine.rendering.common.concepts;

export import :IsRenderBackendLike;
export import :IsRenderResourceHandle;
export import :IsRenderViewHandle;
export import :CanProvideWindowHints;
export import :CanInitializeRenderBackend;