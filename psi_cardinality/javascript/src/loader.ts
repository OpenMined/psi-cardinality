import { CppLibraryInstance } from './types'

export type LoaderFn = () => Promise<{ readonly library: CppLibraryInstance }>

/*
 * Emscripten output contains this callback (onRuntimeInitialized)
 * which fires when the library is fully initialized.
 *
 * We're simply converting this into a promise.
 */
const waitUntilReady = (src: CppLibraryInstance): Promise<void> =>
  new Promise(resolve => (src.onRuntimeInitialized = resolve))

/**
 * Export a default function which instantiates the library
 * @param {Object} bin Emscripten library to initialize
 */
export const Loader = (
  bin: () => CppLibraryInstance
): LoaderFn => async (): Promise<{ readonly library: CppLibraryInstance }> => {
  const library = bin()
  await waitUntilReady(library)
  return {
    library
  }
}
